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

	UWeaponSlot() {
		TypeFilter = UWeaponItem::StaticClass();
	}
};
