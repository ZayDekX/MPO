#include "AmmoStorage.h"

void UAmmoStorage::InitAmmoStorage(int32 InCapacity, UAmmoItemData* InAllowedAmmo, TSubclassOf<UInventorySlot> InSlotClass) {
	Init(1, InSlotClass && InSlotClass->IsChildOf<UAmmoSlot>() ? InSlotClass : UAmmoSlot::StaticClass());
	
	auto Slot = GetCell<UAmmoSlot>(0);
	Slot->AllowedAmmo = InAllowedAmmo;
	Slot->Capacity = InCapacity;
}

void UAmmoStorage::Reload(UInventorySlot* AmmoSlot) {
	if (auto Slot = GetAmmoSlot()) {
		Slot->StackFrom(AmmoSlot);
	}
}