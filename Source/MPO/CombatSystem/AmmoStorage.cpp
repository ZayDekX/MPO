#include "AmmoStorage.h"

void UAmmoStorage::Init() {
	auto InitialCapacity = Capacity;
	SlotClass = SlotClass && SlotClass->IsChildOf<UAmmoSlot>() ? SlotClass : UAmmoSlot::StaticClass();

	Capacity = 1;
	Super::Init();
	Capacity = InitialCapacity;

	auto Slot = Cast<UAmmoSlot>(Content[0]);
	Slot->AllowedAmmo = AllowedAmmo;
	Slot->Capacity = Capacity;
}

void UAmmoStorage::Reload(UInventorySlot* AmmoSlot) {
	if (auto Slot = GetAmmoSlot()) {
		Slot->StackFrom(AmmoSlot);
		return;
	}

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10, FColor::Red, "Magazine content is invalid!");
}

UAmmoSlot* UAmmoStorage::GetAmmoSlot() {
	if (!Content.IsValidIndex(0)) {
		return nullptr;
	}

	auto Slot = Content[0];
	if (Slot->IsValidLowLevel()) {
		return Cast<UAmmoSlot>(Slot);
	}

	return nullptr;
}

UAmmoItem* UAmmoStorage::GetAmmo() {
	auto Slot = GetAmmoSlot();
	if (!Slot) {
		return nullptr;
	}

	return Cast<UAmmoItem>(Slot->Content);
}