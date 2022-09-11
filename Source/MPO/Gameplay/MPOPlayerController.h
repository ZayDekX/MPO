// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPOPlayerController.generated.h"

UCLASS()
class AMPOPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMPOPlayerController();
	
	/* Position in world under player's mouse cursor */
	UPROPERTY(BlueprintReadOnly)
	FVector PointUnderCursorLocation;

	/* Normal vector of the surface under player's mouse cursor */
	UPROPERTY(BlueprintReadOnly)
	FVector UnderCursorSufaceNormal;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FRotator TargetCameraRotation;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* InventoryWidget;

protected:

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	void OnToggleInventory();
	void OnEquipHotbarSlot1();
	void OnEquipHotbarSlot2();

	void OnUseActiveItem();
	void OnEndUseActiveItem();

	void OnChangeFiringMode();

	void OnRotateCameraClockwise();
	void OnRotateCameraCounterClockwise();

	void OnReloadWeapon();

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};