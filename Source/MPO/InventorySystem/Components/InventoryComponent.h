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

private:
	/* Actual item storage */
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UItemStorage* Storage;

	/* Count of slots in storage */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess))
	int32 Capacity;

	/* Class of storage */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess))
	TSubclassOf<UItemStorage> StorageClass;

public:	
	UInventoryComponent();

	void OnRegister() override;

	UItemStorage* GetStorage() { return Storage; }

	template<class T>
	T* GetStorage() { return Cast<T>(Storage); }

	/* Find first slot with content described by provided data */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventorySlot* FindByDataAsset(UBaseItemData* Key);

	/* Find first slot with content described by provided class */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventorySlot* FindByClass(TSubclassOf<UInventoryItem> Key);

	/* Find index of first slot with content described by provided class */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 FindIndexByClass(TSubclassOf<UInventoryItem> Key);

};
