#include "Items/WeaponItem.h"

void UWeaponItem::Init(UBaseItemData* InInfo, int32 InCount) {
    Super::Init(InInfo, InCount);
    auto WeaponData = Cast<UWeaponItemData>(InInfo);

    Magazine = NewObject<UAmmoStorage>(this);
    Magazine->InitAmmoStorage(WeaponData->MagazineSize, WeaponData->AllowedAmmo, UAmmoSlot::StaticClass());
}

void UWeaponItem::Reload(UInventorySlot* AmmoSlot) {
    Magazine->Reload(AmmoSlot);
}

void UWeaponItem::StopUse_Implementation() {
    if (ActualWeapon) {
        ActualWeapon->StopFire();
    }
}

void UWeaponItem::CycleFiringMode() {
    SelectedModeId = (SelectedModeId + 1) % GetInfo<UWeaponItemData>()->ShootingModes.Num();
}

bool UWeaponItem::TryBeUsed_Implementation(UObject* User) {
    if (!(Super::TryBeUsed_Implementation(User) &&
        ActualWeapon &&
        Magazine &&
        !Magazine->IsEmpty()))
    {
        return false;
    }

    auto WeaponInfo = GetInfo<UWeaponItemData>();
    if (!WeaponInfo && WeaponInfo->ShootingModes.IsValidIndex(SelectedModeId)) {
        return false;
    }

    ActualWeapon->BeginFire(WeaponInfo->ShootingModes[SelectedModeId]);
    return true;
}