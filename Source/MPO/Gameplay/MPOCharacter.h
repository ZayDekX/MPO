// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventorySlot.h"
#include "Components/InventoryComponent.h"
#include "Components/CharacterEquipment.h"
#include "Interfaces/ItemUser.h"
#include "MPOCharacter.generated.h"

UCLASS(Blueprintable)
class AMPOCharacter : public ACharacter, public IItemUser
{
    GENERATED_BODY()

public:
    AMPOCharacter();

    /* Point in world space this character should look at */
    UPROPERTY(BlueprintReadOnly)
    FVector LookAt;

    UPROPERTY(BlueprintReadOnly)
    bool IsMoving;

    // Called every frame.
    virtual void Tick(float DeltaSeconds) override;
    
    /** Returns TopDownCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns CursorToWorld subobject **/
    FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
    /** Returns CharacterInventory subobject*/
    FORCEINLINE class UInventoryComponent* GetCharacterInventory() { return CharacterInventory; }
    /** Returns CharacterEquipment subobject*/
    FORCEINLINE class UCharacterEquipment* GetCharacterEquipment() { return CharacterEquipment; }

    FORCEINLINE class UHealthComponent* GetHealthManager() { return HealthManager; }

protected:
    virtual void BeginPlay() override;

private:
    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    /* Inventory */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
    class UInventoryComponent* CharacterInventory;
    
    /* Equipment */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
    class UCharacterEquipment* CharacterEquipment;
    
    /* Health */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
    class UHealthComponent* HealthManager;

    /** A decal that projects to the cursor location. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UDecalComponent* CursorToWorld;

    UFUNCTION()
    virtual void OnDeath();

    UFUNCTION()
    virtual void OnRevive();

public:
};