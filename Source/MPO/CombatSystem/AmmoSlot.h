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

private:

	friend class UAmmoStorage;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int Capacity;
	
public:

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UAmmoItemData* AllowedAmmo;

	virtual void Init(TSubclassOf<UInventoryItem> InTypeFilter) override {
		Super::Init(InTypeFilter && InTypeFilter->IsChildOf<UAmmoItem>() ? InTypeFilter : UAmmoItem::StaticClass());
	}

	virtual int GetMaxStackSize_Implementation() override {
		return Capacity;
	}

	int32 GetContentCount() {
		auto Ammo = GetContent();
		return Ammo ? Ammo->GetCount() : 0;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsFull() { return GetContentCount() == Capacity; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEmpty() { return GetContentCount() == 0; }
};
