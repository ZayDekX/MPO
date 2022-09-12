#include "ItemStorage.h"

void UItemStorage::Init(int32 InCapacity, TSubclassOf<UInventorySlot> InSlotClass) {
    
    InSlotClass = InSlotClass ? InSlotClass : GetDefault<UItemStorage>(GetClass())->SlotClass;
    SlotClass = InSlotClass ? InSlotClass : UInventorySlot::StaticClass();

    Content.Reserve(InCapacity);
    for (auto i = 0; i < InCapacity; i++) {
        auto NewSlot = NewObject<UInventorySlot>(this, SlotClass);
        NewSlot->Init(SlotClass->GetDefaultObject<UInventorySlot>()->GetTypeFilter());
        Content.Add(NewSlot);
    }
}

UInventorySlot* UItemStorage::FindByDataAsset(UBaseItemData* Key) {
    if (!Key) {
        return nullptr;
    }

    for (auto Slot : Content) {
        if (Slot->GetContentInfo() == Key) {
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
        auto SlotContent = Slot->GetContent();
        if (SlotContent && SlotContent->IsA(Key)) {
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
        auto SlotContent = Slot->GetContent();
        if (SlotContent && SlotContent->IsA(Key)) {
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

    auto Slot = Content[SlotId];
    if (Slot) {
        Slot->SetContent(Item);
    }
}

UInventoryItem* UItemStorage::CreateItem(TSubclassOf<UInventoryItem> Class, UBaseItemData* Data, int32 Count) {
    if (!(Class && Data && Count > 0)) {
        return nullptr;
    }

    auto Item = NewObject<UInventoryItem>(this, Class);
    Item->Init(Data, Count);

    return Item;
}