#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "ConsumeableItem.generated.h"

/**
 * Represents any item that can be stored in inventory and consumed on use
 */
UCLASS()
class MPO_API UConsumeableItem : public UInventoryItem
{
	GENERATED_BODY()
	
public:

	virtual bool TryBeUsed_Implementation(UObject* User) override;
};
