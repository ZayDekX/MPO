#pragma once

#include "CoreMinimal.h"
#include "Interfaces/ItemUser.h"
#include "AmmoSlot.h"
#include "Items/WeaponItemData.h"
#include "GameFramework/Actor.h"
#include "Engine/AssetManager.h"
#include "BulletProjectile.h"
#include "GenericWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FWeaponBeginReloadEvent, AGenericWeapon, OnWeaponBeginReload);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FWeaponBeginFireEvent, AGenericWeapon, OnWeaponBeginFire);

UCLASS()
class MPO_API AGenericWeapon : public AActor, public IItemUser
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGenericWeapon();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> AnimationClass;

	UPROPERTY()
	UAmmoSlot* AmmoSlot;

	bool bIsShooting;
	bool bIsReloading;
	bool bReceivedStopCommand;

	int PerformedShots;
	int MaxBurstLength;

	// async load callbacks

	void OnAnimLoaded();
	void OnMeshLoaded();

	/* Item that is stored in owner's inventory and represents this weapon */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess))
	class UWeaponItemData* WeaponData;
public:	
	
	void Init(UWeaponItemData* Item, UAmmoSlot* InAmmoSlot);

	void BeginReload();

	UFUNCTION(BlueprintCallable)
	void EndReload();

	bool CanReload() {
		return !bIsShooting;
	}

	bool CanShoot() {
		return !(bIsReloading || bIsShooting);
	}

	/* Begin shooting sequence */
	void BeginFire(int32 MaxShots);

	/* Stop shooting sequence */
	void StopFire();

	/* End single shot */
	UFUNCTION(BlueprintCallable)
	void EndFire();

	/* Triggered when this weapon begins shooting */
	UPROPERTY(BlueprintAssignable, Category="Interactions")
	FWeaponBeginFireEvent OnWeaponBeginFire;

	/* Triggered when this weapon begins reload */
	UPROPERTY(BlueprintAssignable, Category="Interactions")
	FWeaponBeginReloadEvent OnWeaponBeginReload;
};
