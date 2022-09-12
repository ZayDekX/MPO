#pragma once

#include "CoreMinimal.h"
#include "Items/InventoryItem.h"
#include "AmmoStorage.h"
#include "GenericWeapon.h"
#include "Interfaces/ItemUser.h"
#include "WeaponItem.generated.h"

/**
 * Represents any weapon in inventory
 */
UCLASS()
class MPO_API UWeaponItem : public UInventoryItem, public IItemUser
{
    GENERATED_BODY()

private:
    UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
    int32 SelectedModeId;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess))
    AGenericWeapon* ActualWeapon;

    UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
    UAmmoStorage* Magazine;

public:
    virtual void Init(UBaseItemData* InInfo, int32 InCount) override;

    AGenericWeapon* GetActualWeapon() { return ActualWeapon; }
    
    void SetActualWeapon(AGenericWeapon* NewWeapon) { ActualWeapon = NewWeapon; }

    UAmmoStorage* GetMagazine() { return Magazine; }

    UAmmoItemData* GetAllowedAmmo() { return GetInfo<UWeaponItemData>()->AllowedAmmo; }

    void CycleFiringMode();

    UFUNCTION(BlueprintCallable)
    void Reload(UInventorySlot* AmmoSlot);

    virtual void StopUse_Implementation() override;

    virtual bool TryBeUsed_Implementation(UObject* User) override;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanReload() {
        return Magazine && !Magazine->IsFull();
    }
};
