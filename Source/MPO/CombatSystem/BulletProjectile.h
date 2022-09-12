// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Items/AmmoItemData.h"
#include "Particles/ParticleSystem.h"
#include "BulletProjectile.generated.h"

UCLASS()
class MPO_API ABulletProjectile : public AActor
{
	GENERATED_BODY()
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Collision;

	float Mass;
	int32 Bounces;

	/* Sets how many times bullet can bounce off of a hard surface */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess))
	int32 MaxBounces;

	/* Emitters thar can spawn after hitting hard surface */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess))
	TArray<UParticleSystem*> HitEmitters;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess))
	float InitialSpeedMultiplier;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess))
	float MaxBounceAngle;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess))
	bool bCanPenetrate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess))
	TSubclassOf<UDamageType> DamageType;

	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

	UFUNCTION()
	void OnStop(const FHitResult& ImpactResult);

	UPROPERTY()
	TSoftObjectPtr<UParticleSystem> Tracer;

	UParticleSystem* ABulletProjectile::GetEmitterFromVelocity(float Velocity);

	void OnTracerLoaded();

public:	
	ABulletProjectile();

	void Init(UAmmoItemData* AmmoData, FVector Direction);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	/** Returns ProjectileMovement subobject*/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() { return ProjectileMovement; }

	/** Returns Collision subobject*/
	FORCEINLINE class UBoxComponent* GetCollision() { return Collision; }
};
