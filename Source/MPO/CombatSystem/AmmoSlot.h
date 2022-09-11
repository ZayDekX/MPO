#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "Items/AmmoItem.h"
#include "Items/AmmoItemData.h"
#include "AmmoSlot.generated.h"

/**
 * Special class of inventory slots for ammo
 */
UCLASS()
class MPO_API UAmmoSlot : public UInventorySlot
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int Capacity;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UAmmoItemData* AllowedAmmo;

	UAmmoSlot() {
		TypeFilter = UAmmoItem::StaticClass();
	}

	virtual int GetMaxStackSize() override {
		return Capacity;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsFull() {
		return Content && Content->Count == Capacity;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEmpty() {
		return !Content || Content->Count == 0;
	}
};
