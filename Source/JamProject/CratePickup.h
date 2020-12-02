// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "CratePickup.generated.h"

/**
 * A crate is a pickup that should give the player score points.
 * It should also be affected by gravity and by projectiles to make the game spicy.
 * 
*/
UCLASS()
class JAMPROJECT_API ACratePickup : public APickup
{
	GENERATED_BODY()
public:
	//Set Default values for this Actor's properties
	ACratePickup();	
	
	/** Was collected should destroy the pickup and nothing more
	 * I guess it can be used to either collect the box for the tank or
	 * destroy the box on a projectile hit.
	*/
	//Override the WasCollected Function
	//Use implementation because its a Blueprint native event
	void WasCollected_Implementation() override;
	
	/** Sets the score value for this pickup */
	UFUNCTION(BlueprintNativeEvent)
	void SetScore(float Number);

	/** Get the score value for this pickup */
	UFUNCTION(BlueprintNativeEvent)
	float GetScore() const;

private:
	UPROPERTY(Category = Score, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float score;
};
