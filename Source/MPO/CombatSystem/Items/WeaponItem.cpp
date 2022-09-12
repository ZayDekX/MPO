#include "Items/WeaponItem.h"

void UWeaponItem::Init() {
    auto WeaponData = Cast<UWeaponItemData>(Info);

    Magazine = NewObject<UAmmoStorage>(this);
    Magazine->SlotClass = UAmmoSlot::StaticClass();
    Magazine->AllowedAmmo = WeaponData->AllowedAmmo;
    Magazine->Capacity = WeaponData->MagazineSize;
    Magazine->Init();
}

void UWeaponItem::Reload(UInventorySlot* AmmoSlot) {
    if (Magazine->IsValidLowLevel()) {
        Magazine->Reload(AmmoSlot);

        if (ActualWeapon) {
            ActualWeapon->Ammo = Magazine->GetAmmo();
        }
        return;
    }

    GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10, FColor::Red, "Magazine of weapon is invalid!");
}

void UWeaponItem::StopUse() {
    if (ActualWeapon) {
        ActualWeapon->StopFire();
    }
}

void UWeaponItem::CycleFiringMode() {
    SelectedModeId = (SelectedModeId + 1) % Cast<UWeaponItemData>(Info)->ShootingModes.Num();
}

bool UWeaponItem::TryBeUsed_Implementation(UObject* User) {
    if (!(Super::TryBeUsed_Implementation(User) &&
        ActualWeapon &&
        Magazine &&
        !Magazine->IsEmpty()))
    {
        return false;
    }

    auto WeaponInfo = Cast<UWeaponItemData>(Info);
    if (!WeaponInfo && WeaponInfo->ShootingModes.IsValidIndex(SelectedModeId)) {
        GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10, FColor::Red, "Invalid shooting mode selected");
        return false;
    }

    ActualWeapon->BeginFire(WeaponInfo->ShootingModes[SelectedModeId]);
    return true;
}