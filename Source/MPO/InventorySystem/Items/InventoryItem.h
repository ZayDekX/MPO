#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItemData.h"
#include "InventoryItem.generated.h"

/**
 * Base class for all items stored and operated by inventory
 */
UCLASS(Blueprintable, nonTransient)
class MPO_API UInventoryItem : public UObject
{
    GENERATED_BODY()

private:

    friend class UInventorySlot;

    /* Constant information about this item */
    UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, ExposeOnSpawn))
    UBaseItemData* Info;

protected:
    /* Count of items stored in this "pack" of items */
    UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, ExposeOnSpawn))
    int32 Count;

public:
    UInventoryItem() {}

    UFUNCTION()
    virtual void Init(UBaseItemData* InInfo, int32 InCount) {
        checkSlow(Count > 0)

        this->Info = InInfo;
        this->Count = InCount;
    }

    UBaseItemData* GetItemInfo() { return Info; }

    template<class T>
    T* GetInfo() { return Cast<T>(Info); }

    int32 GetCount() { return Count; }

    /* Check whether this item can stack with other item */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanStackWith(UInventoryItem* Other) {
        return Other && Other != this && Info && Other->Info == Info && Info->MaxStackSize > 1;
    }

    /* Check whether this item can be used by other objects */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanBeUsedBy(UObject* User) {
        return Info->IsInteractionAllowed(User) && Count > 0;
    }

    /* Try to perform an interaction between provided object and this item */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool TryBeUsed(UObject* User);
    virtual bool TryBeUsed_Implementation(UObject* User) 
    {
        return CanBeUsedBy(User);
    }

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void StopUse();
    virtual void StopUse_Implementation() {}
};
