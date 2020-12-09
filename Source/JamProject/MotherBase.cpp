// Fill out your copyright notice in the Description page of Project Settings.


#include "MotherBase.h"
#include "Tank.h"


// Sets default values for this component's properties
UMotherBase::UMotherBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bTankInBase = false;
}


// Called when the game starts
void UMotherBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	bTankInBase = false;
}


// Called every frame
void UMotherBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMotherBase::CheckTankInBase()
{
	//Get any actor close by 
	zone->GetOverlappingActors(ActorsNearby);
	//There's no tank unless proven otherwise
	bTankInBase = false;

	//Check any actor nearby
	for (int32 iNearby= 0; iNearby < ActorsNearby.Num(); ++iNearby)
	{
		//See if its a tank
		ATank* const Subject = Cast<ATank>(ActorsNearby[iNearby]);
		if(Subject)
		{
			bTankInBase = true;
		}
	}
}
