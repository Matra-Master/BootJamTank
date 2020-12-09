// Fill out your copyright notice in the Description page of Project Settings.


#include "CratePickup.h"

//Set Default values for this Actor's properties
ACratePickup::ACratePickup()
{
    PickupMesh->SetSimulatePhysics(true);
    
}

void ACratePickup::WasCollected_Implementation()
{
    //Use base pickup behavior
    Super::WasCollected_Implementation();
    //Destroy the Crate
    Destroy();
}

