// Copyright Epic Games, Inc. All Rights Reserved.

#include "MPOPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "MPOCharacter.h"
#include "Engine/World.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HealthComponent.h"
#include "MPOActions.h"

AMPOPlayerController::AMPOPlayerController()
{
    bShowMouseCursor = true;
    SetTickableWhenPaused(true);
    bShouldPerformFullTickWhenPaused = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMPOPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    FHitResult HitPoint;
    GetHitResultUnderCursor(ECC_Camera, false, HitPoint);

    PointUnderCursorLocation = HitPoint.Location;
    UnderCursorSufaceNormal = HitPoint.ImpactNormal;

    SetControlRotation(UKismetMathLibrary::RInterpTo(GetControlRotation(), TargetCameraRotation, DeltaTime, SmoothTargetViewRotationSpeed));

    // keep updating the destination every tick while desired
    if (bMoveToMouseCursor)
    {
        SetNewMoveDestination(PointUnderCursorLocation);
    }
}

void AMPOPlayerController::SetupInputComponent()
{
    // set up gameplay key bindings
    Super::SetupInputComponent();

    InputComponent->BindAction(ACT_SET_DESTINATION, IE_Pressed, this, &AMPOPlayerController::OnSetDestinationPressed);
    InputComponent->BindAction(ACT_SET_DESTINATION, IE_Released, this, &AMPOPlayerController::OnSetDestinationReleased);
    InputComponent->BindAction(ACT_EQUIP_HOTBAR_1, IE_Pressed, this, &AMPOPlayerController::OnEquipHotbarSlot1);
    InputComponent->BindAction(ACT_EQUIP_HOTBAR_2, IE_Pressed, this, &AMPOPlayerController::OnEquipHotbarSlot2);
    InputComponent->BindAction(ACT_TOGGLE_INVENTORY, IE_Pressed, this, &AMPOPlayerController::OnToggleInventory);
    InputComponent->BindAction(ACT_TOGGLE_PAUSE, IE_Pressed, this, &AMPOPlayerController::OnToggleGamePause);
    InputComponent->BindAction(ACT_RELOAD, IE_Pressed, this, &AMPOPlayerController::OnReloadWeapon);
    InputComponent->BindAction(ACT_USE_ACTIVE_ITEM, IE_Pressed, this, &AMPOPlayerController::OnUseActiveItem);
    InputComponent->BindAction(ACT_USE_ACTIVE_ITEM, IE_Released, this, &AMPOPlayerController::OnEndUseActiveItem);
    InputComponent->BindAction(ACT_ROTATE_CAMERA_COUNTERCLOCKWISE, IE_Pressed, this, &AMPOPlayerController::OnRotateCameraCounterClockwise);
    InputComponent->BindAction(ACT_ROTATE_CAMERA_CLOCKWISE, IE_Pressed, this, &AMPOPlayerController::OnRotateCameraClockwise);
    InputComponent->BindAction(ACT_CHANGE_FIRING_MODE, IE_Pressed, this, &AMPOPlayerController::OnChangeFiringMode);
}

void AMPOPlayerController::BeginPlay() {
    Super::BeginPlay();
    auto Widget = CreateWidget<UUserWidget>(this, InGameUIWidgetClass);
    Widget->AddToViewport();
    UIWidget = Widget;
}

void AMPOPlayerController::OnChangeFiringMode() {
    auto MPOCharacter = Cast<AMPOCharacter>(GetCharacter());
    if (!MPOCharacter->GetHealthManager()->IsAlive()) {
        return;
    }

    MPOCharacter->GetCharacterEquipment()->ChangeFiringMode();
}
void AMPOPlayerController::OnEndUseActiveItem() {
    auto MPOCharacter = Cast<AMPOCharacter>(GetCharacter());
    if (!MPOCharacter->GetHealthManager()->IsAlive()) {
        return;
    }

    MPOCharacter->GetCharacterEquipment()->StopShot();
}

void AMPOPlayerController::OnUseActiveItem() {
    auto MPOCharacter = Cast<AMPOCharacter>(GetCharacter());
    if (!MPOCharacter->GetHealthManager()->IsAlive()) {
        return;
    }

    MPOCharacter->GetCharacterEquipment()->BeginShot();
}

void AMPOPlayerController::OnReloadWeapon() {
    auto MPOCharacter = Cast<AMPOCharacter>(GetCharacter());
    if (!MPOCharacter->GetHealthManager()->IsAlive()) {
        return;
    }

    MPOCharacter->GetCharacterEquipment()->BeginReload();
}

void AMPOPlayerController::OnRotateCameraClockwise()
{
    TargetCameraRotation += FRotator(0, 90, 0);
}

void AMPOPlayerController::OnRotateCameraCounterClockwise()
{
    TargetCameraRotation -= FRotator(0, 90, 0);
}

void AMPOPlayerController::OnToggleInventory() {
    if (!IsPaused()) {
        IMasterUIManager::Execute_TogglePlayerInventory(UIWidget);
    }
}
        
void AMPOPlayerController::OnToggleGamePause() {
    UGameplayStatics::SetGamePaused(this, !IsPaused());
    IMasterUIManager::Execute_TogglePauseMenu(UIWidget);
}

void AMPOPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
    auto MPOCharacter = Cast<AMPOCharacter>(GetCharacter());
    if (!MPOCharacter->GetHealthManager()->IsAlive()) {
        return;
    }

    float const Distance = FVector::Dist(DestLocation, MPOCharacter->GetActorLocation());

    // We need to issue move command only if far enough in order for walk animation to play correctly
    if ((Distance > 120.0f))
    {
        UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
    }
}

void AMPOPlayerController::OnEquipHotbarSlot1()
{
    auto MPOCharacter = Cast<AMPOCharacter>(GetCharacter());
    if (MPOCharacter->GetHealthManager()->IsAlive())
    {
        MPOCharacter->GetCharacterEquipment()->ToggleEquipment(0);
    }
}

void AMPOPlayerController::OnEquipHotbarSlot2()
{
    auto MPOCharacter = Cast<AMPOCharacter>(GetCharacter());
    if (MPOCharacter->GetHealthManager()->IsAlive())
    {
        MPOCharacter->GetCharacterEquipment()->ToggleEquipment(1);
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
