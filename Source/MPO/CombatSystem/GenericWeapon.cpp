#include "GenericWeapon.h"

AGenericWeapon::AGenericWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    SetRootComponent(MeshComponent);
}

void AGenericWeapon::BeginReload() {
    if (!bIsReloading && CanReload()) {
        OnWeaponBeginReload.Broadcast();
        bIsReloading = true;
    }
}

void AGenericWeapon::EndReload() {
    bIsReloading = false;
}

void AGenericWeapon::Init(UWeaponItemData* ItemData, UAmmoSlot* InAmmoSlot) {
    if (!ItemData) {
        return;
    }

    WeaponData = ItemData;
    AmmoSlot = InAmmoSlot;

    auto& StreamableManager = UAssetManager::GetStreamableManager();

    // async load mesh
    if (!WeaponData->Mesh.IsValid()) {
        StreamableManager.RequestAsyncLoad(
            WeaponData->Mesh.ToSoftObjectPath(),
            FStreamableDelegate::CreateUObject(this, &AGenericWeapon::OnMeshLoaded));
    }
    else {
        OnMeshLoaded();
    }

    // async load animation
    if (!WeaponData->AnimationClass.IsValid()) {
        StreamableManager.RequestAsyncLoad(
            WeaponData->AnimationClass.ToSoftObjectPath(),
            FStreamableDelegate::CreateUObject(this, &AGenericWeapon::OnAnimLoaded));
    }
    else {
        OnAnimLoaded();
    }
}

void AGenericWeapon::OnAnimLoaded() {
    MeshComponent->SetAnimInstanceClass(WeaponData->AnimationClass.Get());
}

void AGenericWeapon::OnMeshLoaded() {
    MeshComponent->SetSkeletalMesh(WeaponData->Mesh.Get());
}

void AGenericWeapon::BeginFire(int32 MaxShots) {
    if (MaxShots < 0 || bIsReloading || !(IsValid(AmmoSlot) && AmmoSlot->GetContentCount() > 0 && AmmoSlot->GetContent()->TryBeUsed(this))) {
        PerformedShots = 0;
        return;
    }

    MaxBurstLength = MaxShots;
    bIsShooting = true;

    auto Projectile = GetWorld()->SpawnActorDeferred<ABulletProjectile>(
        WeaponData->ProjectileClass ? WeaponData->ProjectileClass : ABulletProjectile::StaticClass(),
        FTransform(MeshComponent->GetSocketLocation("MuzzleFlash")),
        this,
        GetInstigator(),
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

    Projectile->Init(WeaponData->AllowedAmmo, GetActorRightVector());
    Projectile->FinishSpawning(FTransform(MeshComponent->GetSocketLocation("MuzzleFlash")), true);

    AmmoSlot->OnContentUsed();
    OnWeaponBeginFire.Broadcast();
}

void AGenericWeapon::StopFire() {
    bReceivedStopCommand = true;
}

void AGenericWeapon::EndFire() {
    PerformedShots++;

    if ((MaxBurstLength == 0 && !bReceivedStopCommand) || PerformedShots < MaxBurstLength) {
        BeginFire(MaxBurstLength);
        return;
    }

    MaxBurstLength = -1;
    PerformedShots = 0;
    bIsShooting = false;
    bReceivedStopCommand = false;
}