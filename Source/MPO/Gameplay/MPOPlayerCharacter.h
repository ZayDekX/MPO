#pragma once

#include "CoreMinimal.h"
#include "MPOCharacter.h"
#include "MPOPlayerCharacter.generated.h"

/**
 * Base player character
 */
UCLASS()
class MPO_API AMPOPlayerCharacter : public AMPOCharacter
{
	GENERATED_BODY()

private:
    /** Top down camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UCameraComponent* TopDownCameraComponent;

    /** Camera boom positioning the camera above the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class USpringArmComponent* CameraBoom;

    /** A decal that projects to the cursor location. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
        class UDecalComponent* CursorToWorld;

public:

    AMPOPlayerCharacter();

    virtual void Tick(float DeltaSeconds) override;
    
    virtual FVector GetLookAtPoint_Implementation() override;

    /** Returns TopDownCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns CursorToWorld subobject **/
    FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
    /** Returns CharacterInventory subobject*/
};
