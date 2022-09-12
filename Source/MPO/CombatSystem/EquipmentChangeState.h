#pragma once

#include "CoreMinimal.h"
#include "EquipmentChangeState.generated.h"

/* State of weapon change */
UENUM(BlueprintType)
enum class EEquipmentChangeState : uint8
{
    NoChanges,
    Equip,
    Unequip
};