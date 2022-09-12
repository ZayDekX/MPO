#pragma once

#include "CoreMinimal.h"
#include "ItemStorage.h"
#include "AmmoSlot.h"
#include "AmmoStorage.generated.h"

/**
 * Stores ammo. Primarily used with weapons
 */
UCLASS(Blueprintable)
class MPO_API UAmmoStorage : public UItemStorage
{
    GENERATED_BODY()

private:
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (AllowPrivateAccess))
    UAmmoItemData* AllowedAmmo;

public:
    UAmmoStorage() {}

    virtual void Init(int32 InCapacity, TSubclassOf<UInventorySlot> InSlotClass = nullptr) override { Super::Init(InCapacity, InSlotClass); }
    virtual void InitAmmoStorage(int32 InCapacity, UAmmoItemData* InAllowedAmmo, TSubclassOf<UInventorySlot> InSlotClass = nullptr);

    UAmmoItemData* GetAllowedAmmo() { return AllowedAmmo; }

    /* Refill this slot storage from another ammo container */
    void Reload(UInventorySlot* AmmoSlot);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    UAmmoSlot* GetAmmoSlot() { return GetCell<UAmmoSlot>(0); }

    UAmmoItem* GetAmmo() { return GetCellContent<UAmmoItem>(0); }

    UFUNCTION(BlueprintCallable, BlueprintPure)
        bool IsEmpty() {
        auto Slot = GetAmmoSlot();
        return !IsValid(Slot) || Slot->IsEmpty();
    }

    UFUNCTION(BlueprintCallable, BlueprintPure)
        bool IsFull() {
        auto Slot = GetAmmoSlot();
        return IsValid(Slot) && Slot->IsFull();
    }
};
