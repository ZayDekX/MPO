#pragma once

#include "CoreMinimal.h"
#include "WeaponKind.generated.h"

/*
* Kind of weapon
*/
UENUM(BlueprintType)
enum class EWeaponKind : uint8
{
    Pistol UMETA(DisplayName = "Pistol"),
    Rifle UMETA(DisplayName = "Assault Rifle"),
    SniperRifle UMETA(DisplayName = "Sniper Rifle"),
    Shotgun UMETA(DisplayName = "Shotgun")
};