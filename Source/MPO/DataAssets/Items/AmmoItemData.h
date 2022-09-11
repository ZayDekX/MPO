#pragma once

#include "CoreMinimal.h"
#include "BaseItemData.h"
#include "AmmoItemData.generated.h"

/*
 * Information about ammo
 */
UCLASS()
class MPO_API UAmmoItemData : public UBaseItemData
{
	GENERATED_BODY()

public:
	/*Speed of bullet im meters per second*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BulletStats)
	float Speed;
	
	/*Mass of bullet in gramms*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BulletStats)
	float Mass;

	/*Length of bullet in millimiters*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collision)
	float Length;
	
	/*Diameter of bullet in millimeters*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collision)
	float Diameter;

	/* Can bullet penetrate characters? */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Collision)
	bool bCanPenetrate;

	/*Tracer that will be used to display a bullet flight*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visuals)
	TSoftObjectPtr<UParticleSystem> Tracer;
};
