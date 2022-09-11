#pragma once

#include "CoreMinimal.h"
#include "ItemStorage.h"
#include "AmmoSlot.h"
#include "AmmoStorage.generated.h"

/**
 * Stores ammo. Primarily used with weapons
 */
UCLASS(Blueprintable)
class MPO_API UAmmoStorage : public UItemStorage
{
	GENERATED_BODY()

public:
	UAmmoStorage() {}

	UAmmoItemData* AllowedAmmo;

	virtual void Init() override;

	void Reload(UInventorySlot* AmmoSlot);

	UAmmoSlot* GetAmmoSlot();

	UAmmoItem* GetAmmo();

	bool IsEmpty() {
		auto Slot = GetAmmoSlot();
		return !IsValid(Slot) || Slot->IsEmpty();
	}

	bool IsFull() {
		auto Slot = GetAmmoSlot();
		return IsValid(Slot) && Slot->IsFull();
	}
};
