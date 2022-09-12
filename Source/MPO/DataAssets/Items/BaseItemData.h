#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseItemData.generated.h"

/**
 * Base constant information about any item
 */
UCLASS()
class MPO_API UBaseItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/* Max count of items that can be stored in one slot (does not matter for ammo in weapons) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemInfo)
	int32 MaxStackSize;

	/* Name that should be displayed in UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ItemInfo)
	FText DisplayName;

	/* Model that represents dropped item */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visuals)
	TSoftObjectPtr<UStaticMesh> DroppedMesh;

	/* Describes which objects can use this item */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta=(MustImplement=ItemUser))
	TArray<TSubclassOf<UObject>> AllowedUsers;

	/* Checks whether this info allows to use represented item by provided user */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsInteractionAllowed(UObject* User) {
		if (!User) {
			return false;
		}

		auto UserClass = User->GetClass();
		for (auto AllowedUserClass : AllowedUsers) {
			if (AllowedUserClass && UserClass->IsChildOf(AllowedUserClass)) {
				return true;
			}
		}

		return false;
	}
};
