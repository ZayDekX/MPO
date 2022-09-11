// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/ItemUser.h"
#include "Items/AmmoItem.h"
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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComponent;

private:
	bool bIsShooting;
	bool bIsReloading;
	bool bReceivedStopCommand;

	TSubclassOf<UAnimInstance> AnimationClass;

	void OnAnimLoaded();
	void OnMeshLoaded();

	int PerformedShots;
	int MaxBurstLength;
public:	

	UAmmoItem* Ammo;

	/* Item that is stored in owner's inventory and represents this weapon */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	class UWeaponItemData* WeaponData;
	
	UFUNCTION(BlueprintCallable)
	void InitWith(UWeaponItemData* Item, UAmmoItem* AmmoInfo);

	void BeginReload();

	UFUNCTION(BlueprintCallable)
	void EndReload();

	bool CanReload() {
		return !bIsShooting;
	}

	bool CanShoot() {
		return !(bIsReloading || bIsShooting);
	}

	void BeginFire(int32 MaxShots);

	void StopFire();

	UFUNCTION(BlueprintCallable)
	void EndFire();

	UPROPERTY(BlueprintAssignable, Category="Interactions")
	FWeaponBeginFireEvent OnWeaponBeginFire;

	UPROPERTY(BlueprintAssignable, Category="Interactions")
	FWeaponBeginReloadEvent OnWeaponBeginReload;
};
