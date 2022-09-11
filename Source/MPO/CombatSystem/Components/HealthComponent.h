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

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintReadonly, EditAnywhere)
	float CurrentHealth;

	UPROPERTY(BlueprintReadonly, EditAnywhere)
	float MaxHealth;

	// Called when the game starts
	virtual void BeginPlay() override;

	void ReceiveDamage(float Damage);

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

	UFUNCTION()
	void OnOwnerDamaged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(BlueprintAssignable, Category = Health)
	FDeathEvent OnDeath;

	UPROPERTY(BlueprintAssignable, Category = Health)
	FReviveEvent OnRevive;
};
