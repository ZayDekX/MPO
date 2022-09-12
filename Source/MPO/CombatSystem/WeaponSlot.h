#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "Items/WeaponItem.h"
#include "WeaponSlot.generated.h"

/**
 * Can contain only weapon items
 */
UCLASS()
class MPO_API UWeaponSlot : public UInventorySlot
{
	GENERATED_BODY()
public:

	virtual void Init(TSubclassOf<UInventoryItem> InTypeFilter) override {
		Super::Init(InTypeFilter && InTypeFilter->IsChildOf<UWeaponItem>() ? InTypeFilter : UWeaponItem::StaticClass());
	}
};
