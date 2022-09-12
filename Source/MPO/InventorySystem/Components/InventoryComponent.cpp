#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent() : Super()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::OnRegister()
{
    Super::OnRegister();

    if (StorageClass) {
        Storage = NewObject<UItemStorage>(this, StorageClass);
        Storage->Init(Capacity);
    }
}

UInventorySlot* UInventoryComponent::FindByDataAsset(UBaseItemData* Key) {
    return Storage ? Storage->FindByDataAsset(Key) : nullptr;
}

UInventorySlot* UInventoryComponent::FindByClass(TSubclassOf<UInventoryItem> Key) {
    return Storage ? Storage->FindByClass(Key) : nullptr;
}

int32 UInventoryComponent::FindIndexByClass(TSubclassOf<UInventoryItem> Key) {
    if (!Storage) {
        return -1;
    }

    return Storage->FindIndexByClass(Key);
}