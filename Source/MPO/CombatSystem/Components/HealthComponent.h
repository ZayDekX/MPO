// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FDeathEvent, UHealthComponent, OnDeath);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FReviveEvent, UHealthComponent, OnRevive);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MPO_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintReadonly, EditAnywhere, meta=(AllowPrivateAccess))
	float CurrentHealth;

	UPROPERTY(BlueprintReadonly, EditAnywhere, meta = (AllowPrivateAccess))
	float MaxHealth;

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float Damage);

	UFUNCTION(BlueprintCallable)
	void ReceiveHealing(float Healing, bool Revive);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAlive() {
		return CurrentHealth > 0;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsFullHealth() {
		return FMath::IsNearlyEqual(CurrentHealth, MaxHealth, 0.001f);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsDead() {
		return CurrentHealth < 0.001f;
	}

	/* Takes damage received by owner */
	UFUNCTION()
	void OnOwnerDamaged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	/* Called when CurrentHealth is 0 or less */
	UPROPERTY(BlueprintAssignable, Category = Health)
	FDeathEvent OnDeath;

	/* Called when health restored after death */
	UPROPERTY(BlueprintAssignable, Category = Health)
	FReviveEvent OnRevive;
};
