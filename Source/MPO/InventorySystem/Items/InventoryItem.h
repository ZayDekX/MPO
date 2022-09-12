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
public:

    UInventoryItem() {}

    /* Constant information about this item */
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    UBaseItemData* Info;

    /* Count of items stored in this "pack" of items */
    UPROPERTY(BlueprintReadWrite)
    int32 Count;

    virtual void Init() {

    }

    virtual void BeginDestroy() override {
        Super::BeginDestroy();
    }

    /* Checks whether this item can stack with other item */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanStackWith(UInventoryItem* Other) {
        // if we are stacking not the same instance, both items are valid and can be stacked more than 1 per slot, then we can stack items
        return Other && Other != this && Info && Other->Info == Info && Info->MaxStackSize > 1;
    }

    /* Check whether this item can be used by other objects */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanBeUsedBy(UObject* User) {
        return Info->IsInteractionAllowed(User) && Count > 0;
    }

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    bool TryBeUsed(UObject* User);
    
    virtual void StopUse() {}

    virtual bool TryBeUsed_Implementation(UObject* User) 
    {
        return CanBeUsedBy(User);
    }
};
