#pragma once

#include "CoreMinimal.h"
#include "BaseItemData.h"
#include "HealingItemData.generated.h"

/**
* Information about items that restore character's health
*/
UCLASS()
class MPO_API UHealingItemData : public UBaseItemData
{
	GENERATED_BODY()
public:
	
	/*Defines how much health an item can restore*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Healing)
	float Restore;
	
	/*Does this item reanimate dead players?*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Healing)
	bool bReanimate;
	
	/*Does this item require it's user to be alive?*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Healing)
	bool bOnlyAliveUser;
};
