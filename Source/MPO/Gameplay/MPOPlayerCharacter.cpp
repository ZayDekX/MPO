#include "MPOPlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MPOPlayerController.h"
#include "Materials/Material.h"

AMPOPlayerCharacter::AMPOPlayerCharacter() {
    // Create a camera boom...
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
    CameraBoom->TargetArmLength = 800.f;
    CameraBoom->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
    CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
    CameraBoom->bUsePawnControlRotation = true;

    // Create a camera...
    TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Create a decal in the world to show the cursor's location
    CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
    CursorToWorld->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/Gameplay/M_Cursor_Decal.M_Cursor_Decal'"));
    if (DecalMaterialAsset.Succeeded())
    {
        CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
    }
    CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
    CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
}

void AMPOPlayerCharacter::Tick(float DeltaSeconds) {
    Super::Tick(DeltaSeconds);

    if (auto PlayerController = GetController<AMPOPlayerController>()) {
        CursorToWorld->SetWorldLocation(PlayerController->GetPointUnderCursorLocation());
        CursorToWorld->SetWorldRotation(PlayerController->GetUnderCursorSufaceNormal().Rotation());
    }
}

FVector AMPOPlayerCharacter::GetLookAtPoint_Implementation() {
    if (auto PlayerController = GetController<AMPOPlayerController>()) {
        return PlayerController->GetPointUnderCursorLocation();
    }

    return Super::GetLookAtPoint();
}