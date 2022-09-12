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
        Storage->Capacity = Capacity;
        Storage->Init();
    }
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

UInventorySlot* UInventoryComponent::FindByDataAsset(UBaseItemData* Key) {
    if (!Storage) {
        return nullptr;
    }

    return Storage->FindByDataAsset(Key);
}

UInventorySlot* UInventoryComponent::FindByClass(UClass* Key) {
    if (!Storage) {
        return nullptr;
    }

    return Storage->FindByClass(Key);
}

int32 UInventoryComponent::FindIndexByClass(TSubclassOf<UInventoryItem> Key) {
    if (!Storage) {
        return -1;
    }

    return Storage->FindIndexByClass(Key);
}