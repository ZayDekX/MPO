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

private:	
	/* Position in world under player's mouse cursor */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector PointUnderCursorLocation;

	/* Normal vector of the surface under player's mouse cursor */
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
	FVector UnderCursorSufaceNormal;

	/* Rotation this controller will smoothly turn to */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(AllowPrivateAccess))
	FRotator TargetCameraRotation;

	/* Class of widget that represents In-Game UI */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta=(MustImplement="MasterUIManager", AllowPrivateAccess))
	TSubclassOf<UUserWidget> InGameUIWidgetClass;

	/* In-Game UI */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UUserWidget* UIWidget;

public:
	AMPOPlayerController();

	UUserWidget* GetUIWidget() { return UIWidget; }
	FRotator& GetTargetCameraRotation() { return TargetCameraRotation; }
	FVector& GetUnderCursorSufaceNormal() { return UnderCursorSufaceNormal; }
	FVector& GetPointUnderCursorLocation() { return PointUnderCursorLocation; }

protected:

	/** True if the controlled character should navigate to the mouse cursor. */
	bool bMoveToMouseCursor : 1;
	
	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	// End PlayerController interface

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	// Input event handlers
private:
	
	UFUNCTION(BlueprintCallable)
	void OnToggleInventory();
	
	UFUNCTION(BlueprintCallable)
	void OnToggleGamePause();

	void OnEquipHotbarSlot1();
	void OnEquipHotbarSlot2();
	void OnUseActiveItem();
	void OnEndUseActiveItem();
	void OnChangeFiringMode();
	void OnRotateCameraClockwise();
	void OnRotateCameraCounterClockwise();
	void OnReloadWeapon();
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
};