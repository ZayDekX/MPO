#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MasterUIManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UMasterUIManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * Used for In-Game UI state management
 */
class MPO_API IMasterUIManager
{
	GENERATED_BODY()

public:
	/* Toggle pause menu
	* Called after game is paused or unpaused
	*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TogglePauseMenu();
	
	/* Toggle inventory
	* Called only if game is not paused, when player is trying to toggle inventory state
	*/
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void TogglePlayerInventory();
};
