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

	int Bounces;
	float Mass;
	UParticleSystem* ABulletProjectile::GetEmitterFromVelocity(float Velocity);
	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);
	UFUNCTION()
	void OnStop(const FHitResult& ImpactResult);
	void OnTracerLoaded();

	TSoftObjectPtr<UParticleSystem> Tracer;

public:	
	// Sets default values for this actor's properties
	ABulletProjectile();

	void Init(UAmmoItemData* AmmoData, FVector Direction);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TArray<UParticleSystem*> HitEmitters;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MaxBounces;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float InitialSpeedMultiplier;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxBounceAngle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bCanPenetrate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UDamageType> DamageType;

	/** Returns ProjectileMovement subobject*/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() { return ProjectileMovement; }

	/** Returns Collision subobject*/
	FORCEINLINE class UBoxComponent* GetCollision() { return Collision; }
};
