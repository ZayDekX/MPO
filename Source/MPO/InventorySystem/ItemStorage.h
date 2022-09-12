#pragma once

#include "CoreMinimal.h"
#include "InventorySlot.h"
#include "Interfaces/ItemContainer.h"
#include "UObject/NoExportTypes.h"
#include "ItemStorage.generated.h"

/**
 * Represents an inventory with items
 */
UCLASS(Blueprintable, nonTransient)
class MPO_API UItemStorage : public UObject, public IItemContainer
{
    GENERATED_BODY()

protected:
    TArray<UInventorySlot*> Content;

public:
    UItemStorage(){}

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    int Capacity;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    TSubclassOf<UInventorySlot> SlotClass;

    UFUNCTION(BlueprintCallable)
    virtual void Init();

    /* Find first slot with content described by provided data */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UInventorySlot* FindByDataAsset(UBaseItemData* Key);

    /* Find first slot with content described by provided class */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UInventorySlot* FindByClass(TSubclassOf<UInventoryItem> Key);
    
    /* Find index of first slot with content described by provided class */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    int32 FindIndexByClass(TSubclassOf<UInventoryItem> Key);

    /* Get item from specific slot by slot index */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UInventoryItem* GetCellContent(int32 SlotId) 
    {
        return Content.IsValidIndex(SlotId) ? Content[SlotId]->Content : nullptr;
    }

    /* Set item in specific slot by index*/
    UFUNCTION(BlueprintCallable)
    void SetCellContent(int32 SlotId, UInventoryItem* Item);

    /* Creates an item with specified parameters
    * Only for testing!
    */
    UFUNCTION(BlueprintCallable)
    UInventoryItem* CreateItem(TSubclassOf<UInventoryItem> Class, UBaseItemData* Data, int32 Count);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    TArray<UInventorySlot*>& GetContent() 
    {
        return Content;
    }
};
