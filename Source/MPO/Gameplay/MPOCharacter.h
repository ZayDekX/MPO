// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventorySlot.h"
#include "Components/InventoryComponent.h"
#include "Components/CharacterEquipment.h"
#include "Interfaces/ItemUser.h"
#include "MPOCharacter.generated.h"

/* Base class for characters */
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

    FORCEINLINE class UInventoryComponent* GetCharacterInventory() { return CharacterInventory; }
    /** Returns CharacterEquipment subobject*/
    FORCEINLINE class UCharacterEquipment* GetCharacterEquipment() { return CharacterEquipment; }

    FORCEINLINE class UHealthComponent* GetHealthManager() { return HealthManager; }

    virtual FVector GetLookAtPoint();

protected:
    virtual void BeginPlay() override;

private:

    /* Inventory */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
    class UInventoryComponent* CharacterInventory;
    
    /* Equipment */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
    class UCharacterEquipment* CharacterEquipment;
    
    /* Health */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
    class UHealthComponent* HealthManager;

    UFUNCTION()
    virtual void OnDeath();

    UFUNCTION()
    virtual void OnRevive();

public:
};