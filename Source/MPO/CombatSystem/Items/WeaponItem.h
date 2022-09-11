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

    int32 SelectedModeId;
public:
    virtual void Init() override;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    AGenericWeapon* ActualWeapon;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    UAmmoStorage* Magazine;

    UAmmoItemData* GetAllowedAmmo() {
        return Cast<UWeaponItemData>(Info)->AllowedAmmo;
    }

    void CycleFiringMode();

    UFUNCTION(BlueprintCallable)
    void Reload(UInventorySlot* AmmoSlot);

    virtual void StopUse() override;

    virtual bool TryBeUsed_Implementation(UObject* User) override;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool CanReload() {
        return Magazine && !Magazine->IsFull();
    }
};
