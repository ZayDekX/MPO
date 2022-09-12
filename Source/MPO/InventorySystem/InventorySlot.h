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

private:

	/* Type of allowed items */
	UPROPERTY(BlueprintReadOnly, meta = (ExposedOnSpawn, AllowPrivateAccess))
	TSubclassOf<UInventoryItem> TypeFilter;

	/* Stored item */
	UPROPERTY(BlueprintReadOnly, meta = (ExposedOnSpawn, AllowPrivateAccess))
	UInventoryItem* Content;

public:

	UInventorySlot()
	{}

	virtual void Init(TSubclassOf<UInventoryItem> InTypeFilter) {
		TypeFilter = InTypeFilter;
	}

	TSubclassOf<UInventoryItem> GetTypeFilter() { return TypeFilter; }

	/* Get content of this slot */
	template<class T>
	T* GetContent() { return Cast<T>(Content); }

	/* Get content of this slot */
	UInventoryItem* GetContent() { return Content; }

	/* Get info of content in this slot */
	template<class T>
	T* GetContentInfo() { return Cast<T>(GetContentInfo()); }

	/* Get info of content in this slot */
	UBaseItemData* GetContentInfo() { return Content ? Content->GetItemInfo() : nullptr; }

	/* Check whether this slot can store provided item */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanStore(UInventoryItem* Item) { return !TypeFilter || (Item && Item->IsA(TypeFilter)); }

	/* Check whether this slot can be stacked with other slot */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanStack(UInventorySlot* Other) { 
		return Other && CanStore(Other->Content) && (!Content || Content->CanStackWith(Other->Content));
	}

	/* Check whether this slot contains any items */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasContent();

	/* Take items from other slot and put a remainder back if something is left */
	UFUNCTION(BlueprintCallable)
	void StackFrom(UInventorySlot* Other);

	/* Forces this slot to change it's content to provided item and returns count of remaining items
	* bOverride: allows to change content even if this slot is not empty. (false by default)
	* bIgnoreCapacity: allows to change content by ignoring max capacity defined by item or slot. (false by default)
	* 
	* Types are checked in any case
	*/
	UFUNCTION(BlueprintCallable)
	int32 SetContent(UInventoryItem* Item, bool bOverride = false, bool bIgnoreCapacity = false);

	/* Swaps contents of two cells. Puts remainder back to source cell
	* bForce: swap contents without any checks (leaves no remainder and excess items)
	*/
	UFUNCTION(BlueprintCallable)
	void SwapWith(UInventorySlot* Other, bool bForce = false);

	/* Delete content of this slot */
	UFUNCTION(BlueprintCallable)
	void Clear() {
		Content = nullptr;
	}

	/* Should be called when something have used content of this slot
	* Used for validation
	*/
	UFUNCTION(BlueprintCallable)
	void OnContentUsed();

	/* Clones provided item
	* Count: overrides count of cloned item if this parameter is bigger than 0
	*/
	UFUNCTION(BlueprintCallable)
	UInventoryItem* CloneItem(UInventoryItem* Source, int32 Count);

	/* Gets max size for contained item
	* Can be defined by stored item or overriden in child type
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	int GetMaxStackSize();
	virtual int GetMaxStackSize_Implementation();
};
