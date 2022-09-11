#pragma once

#include "CoreMinimal.h"
#include "Items/InventoryItem.h"
#include "UObject/NoExportTypes.h"
#include "InventorySlot.generated.h"

/**
 * Represents a slot in inventory that can contain items
 */
UCLASS(Blueprintable, nonTransient)
class MPO_API UInventorySlot : public UObject
{
	GENERATED_BODY()

public:

	UInventorySlot()
	{}

	/* Stored item */
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Instanced)
	UInventoryItem* Content;

	/* Type of allowed items */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UInventoryItem> TypeFilter;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanStore(UInventoryItem* Item) {
		return (!Content || Content->CanStackWith(Item)) && (!TypeFilter || Item && Item->GetClass()->IsChildOf(TypeFilter));
	}

	/* Does this slot contains any items? */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasContent();

	/* Take items from other slot and put a remainder back if something is left */
	UFUNCTION(BlueprintCallable)
	void StackFrom(UInventorySlot* Other);

	/* Forces this slot to change it's content to provided item and returns count of remaining items
	* bOverride: allows to change content even if this slot is not empty. (false by default)
	* bIgnoreCapacity: allows to change content by ignoring max capacity defined by item or slot. (false by default)
	*/
	UFUNCTION(BlueprintCallable)
	int32 SetContent(UInventoryItem* Item, bool bOverride = false, bool bIgnoreCapacity = false);

	/* Swaps contents of two cells. Puts remainder back to source cell
	* bForce: swap contents without any checks (leaves no remainder and excess items)
	*/
	UFUNCTION(BlueprintCallable)
	void SwapWith(UInventorySlot* Other, bool bForce = false);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanStack(UInventorySlot* Other) {
		return !HasContent() || Other && Other->HasContent() && CanStore(Other->Content);
	}

	/* Delete content of this slot */
	UFUNCTION(BlueprintCallable)
	void Clear() {
		Content = nullptr;
	}

	/* Called when someone have used content of this slot. 
	* Primarily used for content validation
	*/
	UFUNCTION(BlueprintCallable)
	void OnContentUsed();

	/* Clones provided item
	* Count: overrides count of cloned item if this parameter is bigger than 0
	*/
	UFUNCTION(BlueprintCallable)
	UInventoryItem* CloneItem(UInventoryItem* Source, int32 Count);

	/* Gets max size for contained item. 
	* Can be defined by stored item or overriden in child type 
	*/
	virtual int GetMaxStackSize();
};
