// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPOCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterEquipment.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
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

    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
    //CameraBoom->bUsePawnControlRotation = true;

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Create a decal in the world to show the cursor's location
    CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
    CursorToWorld->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
    if (DecalMaterialAsset.Succeeded())
    {
        CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
    }
    CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
    CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

    // Create inventory
    CharacterInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    CharacterEquipment = CreateDefaultSubobject<UCharacterEquipment>(TEXT("CharacterEquipmentComponent"));

    HealthManager = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthManager"));
    HealthManager->MaxHealth = 5000;
    HealthManager->CurrentHealth = 5000;

    // Activate ticking in order to update the cursor every frame.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMPOCharacter::BeginPlay() {
    Super::BeginPlay();
    HealthManager->OnDeath.AddDynamic(this, &AMPOCharacter::OnDeath);
    HealthManager->OnRevive.AddDynamic(this, &AMPOCharacter::OnRevive);
}

void AMPOCharacter::OnDeath() {
    GetCharacterMovement()->DisableMovement();
    GetController()->SetIgnoreMoveInput(true);
    GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    GetMesh()->SetSimulatePhysics(true);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

void AMPOCharacter::OnRevive() {
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

    auto PlayerController = GetController<AMPOPlayerController>();
    
    if (!PlayerController)
    {
        LookAt = GetActorForwardVector();
    }
    else
    {
        LookAt = PlayerController->PointUnderCursorLocation;

        CursorToWorld->SetWorldLocation(PlayerController->PointUnderCursorLocation);
        CursorToWorld->SetWorldRotation(PlayerController->UnderCursorSufaceNormal.Rotation());
    }

    auto LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookAt);
    auto Delta = UKismetMathLibrary::RInterpTo(GetActorRotation(), LookAtRotation, DeltaSeconds, 10) - GetActorRotation();
    AddActorWorldRotation(FRotator(0, Delta.Yaw, 0));
}
