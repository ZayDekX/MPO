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

private:
    UPROPERTY()
    TArray<UInventorySlot*> Content;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, meta=(AllowPrivateAccess, ExposeOnSpawn))
    TSubclassOf<UInventorySlot> SlotClass;

public:
    UItemStorage(){}

    virtual void Init(int32 InCapacity, TSubclassOf<UInventorySlot> InSlotClass = nullptr);

    template<class T>
    T* GetCell(int32 Index) { return Cast<T>(GetCell(Index)); }

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UInventorySlot* GetCell(int32 Index) {
        return Content.IsValidIndex(Index) ? Content[Index] : nullptr;
    }

    /* Get array of slots available in this storage */
    UFUNCTION(BlueprintCallable, BlueprintPure, meta = (CompactNodeTitle = Cells))
    TArray<UInventorySlot*>& GetCells()
    {
        return Content;
    }

    /* Get count of cells in this storage */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    int32 GetCapacity() {
        return Content.Num();
    }

    /* Get class of slots used in this storage */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    TSubclassOf<UInventorySlot> GetSlotClass() {
        return SlotClass;
    }

    /* Find first slot with content described by provided data */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UInventorySlot* FindByDataAsset(UBaseItemData* Key);

    /* Find first slot with content described by provided class */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UInventorySlot* FindByClass(TSubclassOf<UInventoryItem> Key);
    
    /* Find index of first slot with content described by provided class */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    int32 FindIndexByClass(TSubclassOf<UInventoryItem> Key);

    template<class T>
    T* GetCellContent(int32 SlotId) { return Cast<T>(GetCellContent(SlotId)); }

    /* Get item from specific slot by it's index */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UInventoryItem* GetCellContent(int32 SlotId) 
    {
        return Content.IsValidIndex(SlotId) ? Content[SlotId]->GetContent() : nullptr;
    }

    /* Set item in specific slot by index */
    UFUNCTION(BlueprintCallable)
    void SetCellContent(int32 SlotId, UInventoryItem* Item);

    /* 
    * Create item with specified parameters
    */
    UFUNCTION(BlueprintCallable)
    UInventoryItem* CreateItem(TSubclassOf<UInventoryItem> Class, UBaseItemData* Data, int32 Count);
};
