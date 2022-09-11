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

UInventorySlot* UInventoryComponent::Find(UBaseItemData* Key) {
    if (!Storage) {
        return nullptr;
    }

    return Storage->Find(Key);
}