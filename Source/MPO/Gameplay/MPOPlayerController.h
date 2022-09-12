// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MasterUIManager.h"
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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(MustImplement="MasterUIManager"))
	TSubclassOf<UUserWidget> InGameUIWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* UIWidget;

protected:

	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	// End PlayerController interface

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	UFUNCTION(BlueprintCallable)
	void OnToggleInventory();

	UFUNCTION(BlueprintCallable)
	void OnToggleGamePause();

	UFUNCTION()
	void OnEquipHotbarSlot1();

	UFUNCTION()
	void OnEquipHotbarSlot2();

	UFUNCTION()
	void OnUseActiveItem();

	UFUNCTION()
	void OnEndUseActiveItem();

	UFUNCTION()
	void OnChangeFiringMode();

	UFUNCTION()
	void OnRotateCameraClockwise();
	
	UFUNCTION()
	void OnRotateCameraCounterClockwise();

	UFUNCTION()
	void OnReloadWeapon();

	/** Input handlers for SetDestination action. */
	UFUNCTION()
	void OnSetDestinationPressed();

	UFUNCTION()
	void OnSetDestinationReleased();
};