#include "Components/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay() {
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnOwnerDamaged);
}

void UHealthComponent::ReceiveDamage(float Damage) {
	if (IsDead()) {
		return;
	}

	CurrentHealth = CurrentHealth - Damage;

	if (IsDead()) {
		CurrentHealth = 0;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::ReceiveHealing(float Healing, bool Revive) {
	if (IsAlive()) {
		CurrentHealth = FMath::Clamp(CurrentHealth + Healing, 0.0f, MaxHealth);
		return;
	}

	if (Revive) {
		CurrentHealth = FMath::Clamp(Healing, 0.0f, MaxHealth);
		OnRevive.Broadcast();
	}
}

void UHealthComponent::OnOwnerDamaged(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	ReceiveDamage(Damage);
}