#pragma once

#include "CoreMinimal.h"
#include "ItemStorage.h"
#include "WeaponSlot.h"
#include "Items/WeaponItem.h"
#include "WeaponStorage.generated.h"

/**
 * Stores only weapons
 */
UCLASS()
class MPO_API UWeaponStorage : public UItemStorage
{
	GENERATED_BODY()
	
	virtual void Init() override {
		SlotClass = UWeaponSlot::StaticClass();
		Super::Init();
	}
};
