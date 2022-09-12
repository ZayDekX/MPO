#include "ItemStorage.h"

void UItemStorage::Init() {
    if (!Capacity) {
        return;
    }
    
    Content.Init(nullptr, Capacity);
    for (auto i = 0; i < Capacity; i++) {
        Content[i] = NewObject<UInventorySlot>(this, SlotClass ? SlotClass : UInventorySlot::StaticClass());
    }
}

UInventorySlot* UItemStorage::FindByDataAsset(UBaseItemData* Key) {
    if (!Key) {
        return nullptr;
    }

    for (auto Slot : Content) {
        if (Slot->Content && Slot->Content->Info == Key) {
            return Slot;
        }
    }

    return nullptr;
}

UInventorySlot* UItemStorage::FindByClass(TSubclassOf<UInventoryItem> Key) {
    if (!Key) {
        return nullptr;
    }

    for (auto Slot : Content) {
        if (Slot->Content && Slot->Content->GetClass()->IsChildOf(Key)) {
            return Slot;
        }
    }

    return nullptr;
}

int32 UItemStorage::FindIndexByClass(TSubclassOf<UInventoryItem> Key) {
    if (!Key) {
        return -1;
    }

    auto Index = 0;

    for (auto Slot : Content) {
        if (Slot->Content && Slot->Content->GetClass()->IsChildOf(Key)) {
            return Index;
        }
        Index++;
    }

    return -1;
}

void UItemStorage::SetCellContent(int32 SlotId, UInventoryItem* Item)
{
    if (!Content.IsValidIndex(SlotId)) {
        return;
    }

    Content[SlotId]->SetContent(Item);
}

UInventoryItem* UItemStorage::CreateItem(TSubclassOf<UInventoryItem> Class, UBaseItemData* Data, int32 Count) {
    if (!(Class && Data)) {
        return nullptr;
    }

    auto Item = NewObject<UInventoryItem>(this, Class);
    Item->Info = Data;
    Item->Count = Count;
    Item->Init();
    return Item;
}