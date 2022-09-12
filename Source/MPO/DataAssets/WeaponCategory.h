#pragma once

#include "CoreMinimal.h"
#include "WeaponCategory.generated.h"

/*
* Category of a weapon
*/
UENUM(BlueprintType)
enum class EWeaponCategory : uint8
{
    Primary,
    Secondary
};