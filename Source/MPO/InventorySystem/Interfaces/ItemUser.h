#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Items/InventoryItem.h"
#include "ItemUser.generated.h"

UINTERFACE(MinimalAPI)
class UItemUser : public UInterface
{
	GENERATED_BODY()
};

/*
* Primarily used to mark objects that can use items and provide similar API in all users
*/
class MPO_API IItemUser
{
	GENERATED_BODY()
public:
	/* Try to perform an interaction with provided item */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool TryUse(UInventoryItem* Item);
	virtual bool TryUse_Implementation(UInventoryItem* Item) {
		return Item->TryBeUsed(Cast<UObject>(this));
	}

	/* Check whether it's possible to interact with provided item */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanUse(UInventoryItem* Item);
	bool CanUse_Implementation(UInventoryItem* Item) {
		return Item->CanBeUsedBy(Cast<UObject>(this));
	}
};
