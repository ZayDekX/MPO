#pragma once

#include "CoreMinimal.h"
#include "BaseItemData.h"
#include "WeaponKind.h"
#include "WeaponCategory.h"
#include "AmmoItemData.h"
#include "BulletProjectile.h"
#include "WeaponItemData.generated.h"

/*
* Stores information about weapon
*/
UCLASS()
class MPO_API UWeaponItemData : public UBaseItemData
{
    GENERATED_BODY()
public:
    /* Kind of weapon */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponInfo)
    EWeaponKind Kind;

    /* Category of weapon */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponInfo)
    EWeaponCategory Category;

    /* 
    * Defines modes of shooting that can be applied to described weapon 
    * Each mode describes how many bullets can be in a single burst, while selected mode is active:
    * 0 - auto
    * 1 - semi-auto
    * 2 or more - burst
    */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = WeaponInfo)
    TArray<int32> ShootingModes;

    /* Ammo that can be used by weapon */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AmmoSlot)
    UAmmoItemData* AllowedAmmo;

    /* Max count of bullets that is stored in magazine */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AmmoSlot)
    int32 MagazineSize;

    /* Class of projectiles created by this weapon */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
    TSubclassOf<ABulletProjectile> ProjectileClass = ABulletProjectile::StaticClass();

    /* Mesh that represents a weapon in character's hands */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visuals, meta=(AssetBundles="Weapon Skeletons"))
    TSoftObjectPtr<USkeletalMesh> Mesh;

    /* Animation blueprint that is used by mesh */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visuals, meta=(AssetBundles="Weapon Animations"))
    TSoftClassPtr<UAnimInstance> AnimationClass;
};