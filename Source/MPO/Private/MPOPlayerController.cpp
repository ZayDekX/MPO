// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPOPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "MPOCharacter.h"
#include "Engine/World.h"
#include "MPOActions.h"

AMPOPlayerController::AMPOPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMPOPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // keep updating the destination every tick while desired
    if (bMoveToMouseCursor)
    {
        MoveToMouseCursor();
    }
}

void AMPOPlayerController::SetupInputComponent()
{
    // set up gameplay key bindings
    Super::SetupInputComponent();

    InputComponent->BindAction(ACT_SET_DESTINATION, IE_Pressed, this, &AMPOPlayerController::OnSetDestinationPressed);
    InputComponent->BindAction(ACT_SET_DESTINATION, IE_Released, this, &AMPOPlayerController::OnSetDestinationReleased);
}

void AMPOPlayerController::MoveToMouseCursor()
{
    // Trace to see what is under the mouse cursor
    FHitResult Hit;
    GetHitResultUnderCursor(ECC_Visibility, false, Hit);

    if (Hit.bBlockingHit)
    {
        // We hit something, move there
        SetNewMoveDestination(Hit.ImpactPoint);
    }
}

void AMPOPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
    APawn* const MyPawn = GetPawn();
    if (!MyPawn)
    {
        return;
    }

    float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

    // We need to issue move command only if far enough in order for walk animation to play correctly
    if ((Distance > 120.0f))
    {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
    }
}

void AMPOPlayerController::OnSetDestinationPressed()
{
    // set flag to keep updating destination until released
    bMoveToMouseCursor = true;
}

void AMPOPlayerController::OnSetDestinationReleased()
{
    // clear flag to indicate we should stop updating the destination
    bMoveToMouseCursor = false;
}
