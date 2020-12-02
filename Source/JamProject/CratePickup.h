// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "CratePickup.generated.h"

/**
 * 
 */
UCLASS()
class JAMPROJECT_API ACratePickup : public APickup
{
	GENERATED_BODY()
public:
	//Set Default values for this Actor's properties
	ACratePickup();	
	
	//Override the WasCollected Function
	//Use implementation because its a Blueprint native event
	void WasCollected_Implementation() override;
	
};
