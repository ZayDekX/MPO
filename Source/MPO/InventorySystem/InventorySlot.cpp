#include "InventorySlot.h"

bool UInventorySlot::HasContent()
{
	return Content && Content->GetCount() > 0;
}

void UInventorySlot::StackFrom(UInventorySlot* Other) {
	if (!(Other && CanStore(Other->Content))) {
		return;
	}
	
	if (!HasContent()) {
		if (auto Remainder = SetContent(Other->Content)) {
			Other->Content = CloneItem(Content, Remainder);
		}
		else {
			Other->Clear();
		}
		return;
	}

	auto TotalCount = Content->GetCount() + Other->Content->GetCount();
	Content->Count = FMath::Min(GetMaxStackSize(), TotalCount);

	Other->Content->Count = TotalCount - Content->Count;

	// if external object is empty, it needs to be deleted
	if (!Other->HasContent()) {
		Other->Clear();
	}
}

int32 UInventorySlot::SetContent(UInventoryItem* Item, bool bOverride, bool bIgnoreCapacity) {
	if (!Item) {
		if (bOverride) {
			Content = nullptr;
		}

		return 0;
	}

	if (!CanStore(Item)) {
		return Item->Count;
	}

	// return incoming item if we can't override content
	if (HasContent() && !bOverride) {
		return Item->Count;
	}

	Content = Item;

	if (bIgnoreCapacity) {
		return 0;
	}

	auto MaxCount = GetMaxStackSize();
	auto SourceCount = Content->Count;

	if (SourceCount > MaxCount) {
		Content->Count = MaxCount;
		return SourceCount - MaxCount;
	}

	return 0;
}

void UInventorySlot::SwapWith(UInventorySlot* Other, bool bForce) {
	if (!(Other && CanStore(Other->Content))) {
		return;
	}

	auto ExternalContent = Other->Content;

	if (bForce) {
		Other->SetContent(Content, true, true);
		this->SetContent(ExternalContent, true, true);
		return;
	}

	// change content of external cell
	if (HasContent()) {
		Other->SetContent(Content, true);
	}
	else {
		Other->Clear();
	}

	auto Remainder = this->SetContent(ExternalContent, true);

	if (Remainder && !Other->HasContent()) {
		// put remainder back to source cell, but discard a remainder
		// remainder can come only if we have unfair player
		Other->SetContent(CloneItem(Content, Remainder), true);
	}
}

UInventoryItem* UInventorySlot::CloneItem(UInventoryItem* Source, int32 Count = 0) {
	auto Item = NewObject<UInventoryItem>(this, Source->GetClass());
	Item->Init(Source->Info, Count > 0 ? Count : Source->Count);

	return Item;
}

void UInventorySlot::OnContentUsed() {
	if (!HasContent()) {
		return;
	}

	if (Content->Count <= 0) {
		Content = nullptr;
	}
}

int32 UInventorySlot::GetMaxStackSize_Implementation() {
	if (!HasContent()) {
		return 0;
	}

	return Content->Info->MaxStackSize;
}