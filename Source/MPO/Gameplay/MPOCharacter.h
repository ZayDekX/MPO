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

private:

    /* Point in world space this character should look at */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    FVector LookAt;

    /* Does this character moving on current tick? */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
    bool IsMoving;

    /* Inventory */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
    class UInventoryComponent* CharacterInventory;

    /* Equipment */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
    class UCharacterEquipment* CharacterEquipment;

    /* Health */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
    class UHealthComponent* HealthManager;

protected:
    virtual void BeginPlay() override;

public:
    AMPOCharacter();

    // Called every frame.
    virtual void Tick(float DeltaSeconds) override;

    /** Returns CharacterInventory subobject*/
    FORCEINLINE class UInventoryComponent* GetCharacterInventory() { return CharacterInventory; }
    /** Returns CharacterEquipment subobject*/
    FORCEINLINE class UCharacterEquipment* GetCharacterEquipment() { return CharacterEquipment; }
    /** Returns HealthManager subobject*/
    FORCEINLINE class UHealthComponent* GetHealthManager() { return HealthManager; }

    UFUNCTION(BlueprintNativeEvent)
    FVector GetLookAtPoint();
    virtual FVector GetLookAtPoint_Implementation();

public:
    /* Called on death */
    UFUNCTION(BlueprintNativeEvent)
    void OnDeath();
    virtual void OnDeath_Implementation();

    /* Called on revive */
    UFUNCTION(BlueprintNativeEvent)
    void OnRevive();
    virtual void OnRevive_Implementation();
};