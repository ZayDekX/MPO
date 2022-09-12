#include "Components/CharacterEquipment.h"
#include "MPOCharacter.h"

void UCharacterEquipment::ToggleEquipment(int32 NewSlot)
{
    if (!CanMakeActions()) {
        return;
    }

    if (ActiveSlot == NewSlot) {
        BeginUnequip();
        return;
    }

    if (!EquippedItem) {
        BeginEquip(NewSlot);
        return;
    }

    bIsSwappingEquipment = true;

    BeginUnequip();
    ActiveSlot = NewSlot;
}

void UCharacterEquipment::BeginShot() {
    if (CanMakeActions() && EquippedItem && EquippedItem->TryBeUsed(GetOwner())) {
        OnShotBegin.Broadcast();
        bIsShooting = true;
    }
}

void UCharacterEquipment::StopShot() {
    if (EquippedItem) {
        EquippedItem->StopUse();
    }
}

void UCharacterEquipment::BeginUnequip()
{
    if (EquippedItem) {

        bIsChangingEquipment = true;

        ChangeState = EEquipmentChangeState::Unequip;
        OnUnequipBegin.Broadcast();
    }
    ActiveSlot = -1;
}

void UCharacterEquipment::EndUnequip() {

    bIsChangingEquipment = false;

    if (bIsSwappingEquipment) {
        bIsSwappingEquipment = false;
        BeginEquip(ActiveSlot);
    }
}

void UCharacterEquipment::EndEquip() {
    bIsChangingEquipment = false;
    OnEquipEnd.Broadcast(EquippedItem);
}

void UCharacterEquipment::BeginReload() {
    if (!(CanMakeActions() && EquippedItem && EquippedItem->CanReload())) {
        return;
    }

    auto Character = GetOwner<AMPOCharacter>();
    if (!Character) {
        return;
    }
    AmmoSlot = Character->GetCharacterInventory()->FindByDataAsset(EquippedItem->GetAllowedAmmo());
    if (!AmmoSlot) {
        return;
    }

    bIsReloading = true;

    OnReloadBegin.Broadcast();
    EquippedItem->ActualWeapon->BeginReload();
}

void UCharacterEquipment::EndReload() {
    if (!(EquippedItem && EquippedItem->CanReload())) {
        return;
    }

    EquippedItem->Reload(AmmoSlot);
    AmmoSlot = nullptr;
    bIsReloading = false;
    OnReloadEnd.Broadcast();
}

void UCharacterEquipment::BeginEquip(int32 NewSlot)
{
    auto StoredItem = Storage->GetCellContent(NewSlot);
    if (!StoredItem) {
        ActiveSlot = -1;
        return;
    }

    EquipTarget = Cast<UWeaponItem>(StoredItem);
    if (!EquipTarget) {
        return;
    }

    ActiveSlot = NewSlot;
    ChangeState = EEquipmentChangeState::Equip;
    bIsChangingEquipment = true;

    OnEquipBegin.Broadcast(Cast<UWeaponItemData>(EquipTarget->Info)->Kind);
}

void UCharacterEquipment::ChangeWeapon() {
    static auto DetachRules = FDetachmentTransformRules(EDetachmentRule::KeepRelative, false);
    static auto AttachmentRules = FAttachmentTransformRules(
        EAttachmentRule::SnapToTarget,
        EAttachmentRule::KeepRelative,
        EAttachmentRule::KeepRelative,
        true);

    switch (ChangeState)
    {
    case EEquipmentChangeState::Equip:
    {
        auto WeaponInfo = Cast<UWeaponItemData>(EquipTarget->Info);
        FName SocketName = WeaponInfo->Category == EWeaponCategory::Secondary ? "secondary_weapon_socket" : "primary_weapon_socket";

        if (EquippedItem && EquippedItem->ActualWeapon) {
            EquippedItem->ActualWeapon->DetachFromActor(DetachRules);
        }

        if (EquipTarget) {
            EquipTarget->ActualWeapon =
                GetWorld()->SpawnActorDeferred<AGenericWeapon>(
                    AGenericWeapon::StaticClass(),
                    FTransform::Identity,
                    GetOwner(),
                    GetOwner<APawn>());

            EquipTarget->ActualWeapon->InitWith(WeaponInfo, EquipTarget->Magazine->GetAmmo());
            EquipTarget->ActualWeapon->AttachToComponent(GetOwner<ACharacter>()->GetMesh(), AttachmentRules, SocketName);
            EquipTarget->ActualWeapon->FinishSpawning(FTransform::Identity, true);

            EquippedItem = EquipTarget;
            EquipTarget = nullptr;
        };

        break;
    }
    case EEquipmentChangeState::Unequip:
    {
        if (!EquippedItem) {
            break;
        }

        if (auto Weapon = EquippedItem->ActualWeapon) {
            Weapon->DetachFromActor(DetachRules);
            EquippedItem->ActualWeapon = nullptr;
            EquippedItem = nullptr;
            Weapon->Destroy();
        }
        break;
    }
    default:
        return;
    }

    ChangeState = EEquipmentChangeState::NoChanges;
}