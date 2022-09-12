#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComponent.h"
#include "EquipmentChangeState.h"
#include "Items/WeaponItem.h"
#include "WeaponKind.h"
#include "WeaponCategory.h"
#include "Items/WeaponItemData.h"
#include "GameFramework/Character.h"
#include "Interfaces/ItemUser.h"
#include "CharacterEquipment.generated.h"


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FEquipBeginEvent, UCharacterEquipment, OnEquipBegin, EWeaponKind, WeaponKind);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FEquipEndEvent, UCharacterEquipment, OnEquipEnd, UWeaponItem*, EquippedItem);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FUnequipBeginEvent, UCharacterEquipment, OnUnequipBegin);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FReloadBeginEvent, UCharacterEquipment, OnReloadBegin);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FReloadEndEvent, UCharacterEquipment, OnReloadEnd);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FShotBeginEvent, UCharacterEquipment, OnShotBegin);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FShotEndEvent, UCharacterEquipment, OnShotEnd);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FChangedFiringModeEvent, UCharacterEquipment, OnChangedFiringMode);

/**
 * Represents character's equipment.
 * Adds a hotbar and allows interaction with currently equipped item
 */
UCLASS(Blueprintable, classGroup = "Inventory|Equipment", meta = (BlueprintSpawnableComponent))
class MPO_API UCharacterEquipment : public UInventoryComponent
{
    GENERATED_BODY()

private:

    EEquipmentChangeState ChangeState;

    int32 ActiveSlot = -1;

    UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
    UWeaponItem* EquippedItem;

    UPROPERTY()
    UWeaponItem* EquipTarget;
    
    bool bIsSwappingEquipment;
    bool bIsChangingEquipment;
    bool bIsReloading;
    bool bIsShooting;

    UPROPERTY()
    UInventorySlot* AmmoSlot;

public:

    bool CanMakeActions() {
        return !(bIsReloading || bIsShooting || bIsChangingEquipment || bIsSwappingEquipment);
    }

    void ChangeFiringMode() {
        if (EquippedItem) {
            EquippedItem->CycleFiringMode();
            OnChangedFiringMode.Broadcast();
        }
    }

    UFUNCTION(BlueprintCallable)
    void ToggleEquipment(int32 NewSlot);

    UFUNCTION(BlueprintCallable)
    void BeginEquip(int32 NewSlot);

    UFUNCTION(BlueprintCallable)
    void EndEquip();

    UFUNCTION(BlueprintCallable)
    void BeginUnequip();

    UFUNCTION(BlueprintCallable)
    void EndUnequip();
    
    UFUNCTION(BlueprintCallable)
    void BeginReload();

    UFUNCTION(BlueprintCallable)
    void EndReload();

    UFUNCTION(BlueprintCallable)
    void ChangeWeapon();

    UFUNCTION(BlueprintCallable)
    void StopShot();

    UFUNCTION(BlueprintCallable)
    void BeginShot();

    UFUNCTION(BlueprintCallable)
        void EndShot() {
        bIsShooting = false;
    }

    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FEquipBeginEvent OnEquipBegin;

    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FEquipEndEvent OnEquipEnd;

    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FUnequipBeginEvent OnUnequipBegin;

    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FReloadBeginEvent OnReloadBegin;
    
    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FReloadEndEvent OnReloadEnd;

    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FShotBeginEvent OnShotBegin;
    
    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FShotEndEvent OnShotEnd;
    
    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FChangedFiringModeEvent OnChangedFiringMode;
};
