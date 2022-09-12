// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPOCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterEquipment.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/HealthComponent.h"
#include <MPOPlayerController.h>

AMPOCharacter::AMPOCharacter()
{
    // Set size for player capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate character to camera direction
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
    GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Create inventory
    CharacterInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    CharacterEquipment = CreateDefaultSubobject<UCharacterEquipment>(TEXT("CharacterEquipmentComponent"));

    // Create health manager
    HealthManager = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthManager"));

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMPOCharacter::BeginPlay() {
    Super::BeginPlay();
    HealthManager->OnDeath.AddDynamic(this, &AMPOCharacter::OnDeath);
    HealthManager->OnRevive.AddDynamic(this, &AMPOCharacter::OnRevive);
}

void AMPOCharacter::OnDeath_Implementation() {
    GetCharacterMovement()->DisableMovement();
    GetController()->SetIgnoreMoveInput(true);
    GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AMPOCharacter::OnRevive_Implementation() {
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
    GetController()->SetIgnoreMoveInput(false);
    auto DetachedMesh = GetMesh();
    DetachedMesh->SetSimulatePhysics(false);
    DetachedMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DetachedMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    DetachedMesh->SetRelativeTransform(Cast<AMPOCharacter>(GetClass()->GetDefaultObject())->GetMesh()->GetRelativeTransform());
}

void AMPOCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    IsMoving = GetVelocity().Size() > 0;
    LookAt = GetLookAtPoint();
    
    auto LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAt);
    auto Delta = UKismetMathLibrary::RInterpTo(GetActorRotation(), LookAtRotation, DeltaSeconds, 10) - GetActorRotation();
    AddActorWorldRotation(FRotator(0, Delta.Yaw, 0));
}

FVector AMPOCharacter::GetLookAtPoint_Implementation() {
    return GetActorForwardVector();
}