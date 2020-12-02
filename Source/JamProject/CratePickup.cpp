// Fill out your copyright notice in the Description page of Project Settings.


#include "CratePickup.h"

//Set Default values for this Actor's properties
ACratePickup::ACratePickup()
{
    PickupMesh->SetSimulatePhysics(true);
    //The score inherent to an object should be assigned by the gama mode
    //Initialize it with a default value
    score = 100;
    
}

void ACratePickup::WasCollected_Implementation()
{
    //Use base pickup behavior
    Super::WasCollected_Implementation();
    //Destroy the Crate
    Destroy();
}

void APickup::SetScore (float Number)
{
	score = number;
}

float APickup::GetScore() const
{
	return score;
}