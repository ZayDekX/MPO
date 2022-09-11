#pragma once

#include "CoreMinimal.h"
#include "ItemStorage.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

/* Represents a storage for items that can be accessed by character */
UCLASS(Blueprintable, nonTransient, classGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class MPO_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	void BeginPlay() override;

	void OnRegister() override;

protected:

public:	

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsInteractive() {
		return true;
	}

	UInventorySlot* Find(UBaseItemData* Key);

	/* Actual item storage */
	UPROPERTY(BlueprintReadOnly)
	UItemStorage* Storage;

	/* Count of slots in storage */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 Capacity;

	/* Class of storage */
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UItemStorage> StorageClass;
};
