// Fill out your copyright notice in the Description page of Project Settings.


#include "MotherBase.h"


// Sets default values for this component's properties
UMotherBase::UMotherBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMotherBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMotherBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

