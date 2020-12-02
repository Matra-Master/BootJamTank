// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"


// Sets default values
APickup::APickup()
{
	//Create the Static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//All pickups start active
	bIsActive = true;

	RootComponent = PickupMesh;
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Function to get the value of IsActive
bool APickup::IsActive()
{
	return bIsActive;
}

//Procedure to set the value of IsActive
void APickup::SetActive(bool newPickupState)
{
	bIsActive = newPickupState;
	//Put here the other changes that occur when changing the active state
	//Like efects, animations and whatnot
}

//IT seems this is not logging properly
//Maybe its an old macro
void APickup::WasCollected_Implementation()
{
	//Log a debugging message 
	FString PickupDebugString = GetName();
	UE_LOG(LogClass, Log, TEXT("You have collected %s"), *PickupDebugString);
}
