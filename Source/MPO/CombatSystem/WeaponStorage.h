#pragma once

#include "CoreMinimal.h"
#include "ItemStorage.h"
#include "WeaponSlot.h"
#include "Items/WeaponItem.h"
#include "WeaponStorage.generated.h"

/**
 * Stores only weapons
 */
UCLASS()
class MPO_API UWeaponStorage : public UItemStorage
{
	GENERATED_BODY()
	virtual void Init(int32 InCapacity, TSubclassOf<UInventorySlot> InSlotClass = nullptr) override
	{
		Super::Init(InCapacity, InSlotClass && InSlotClass->IsChildOf<UWeaponSlot>() ? InSlotClass : UWeaponSlot::StaticClass());
	}
};
