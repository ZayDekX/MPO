#include "ConsumeableItem.h"

bool UConsumeableItem::TryBeUsed_Implementation(UObject* User)
{
	if (Super::TryBeUsed_Implementation(User)) {
		Count--;
		return true;
	}

	return false;
}
