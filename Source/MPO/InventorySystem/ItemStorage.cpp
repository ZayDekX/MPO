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

UInventorySlot* UItemStorage::Find(UBaseItemData* Key) {
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