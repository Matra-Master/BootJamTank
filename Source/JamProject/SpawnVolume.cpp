// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Math/UnrealMathUtility.h"
#include "Math/TransformNonVectorized.h"
#include "Math/Vector.h"

#include "TankProjectile.h"
#include "Pickup.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the box component to represent the spawn volume
	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawn"));
	RootComponent = WhereToSpawn;
	
	//Set the Pickup spawn delay range
	PickupSpawnDelayRangeLow = 1.0f;
	PickupSpawnDelayRangeHigh = 4.5f;

	//Set the Projectile spawn delay range
	ProjectileDelayRangeHigh = 1.0f;
	ProjectileDelayRangeLow = 0.5f;

	//This wont be used yet
	//ProjectileShootingNumber = 1;
	
	ShootDirection = FVector::UpVector;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	//Set initial timers for shooting and spawning with their respective delays
	SpawnDelay = FMath::FRandRange(PickupSpawnDelayRangeLow, PickupSpawnDelayRangeHigh);
	ShootDelay = FMath::FRandRange(ProjectileDelayRangeLow, ProjectileDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
	GetWorldTimerManager().SetTimer(ShootingTimer, this, &ASpawnVolume::SpawnShots, ShootDelay, false);
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	return FMath::RandPointInBox(GetComponentsBoundingBox(true, false));
}

class UBoxComponent* ASpawnVolume::GetWhereToSpawn()
{
	return WhereToSpawn;
}

void ASpawnVolume::SpawnPickup()
{
	//If we have set something to spawn:
	if(WhatPickupToSpawn)
	{
		//Check for valid world:
		UWorld* const World = GetWorld();
		if(World)
		{
			//Set the spawn parameters:
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			//Get a random location to spawn the pickup
			FVector SpawnLocation = GetRandomPointInVolume();
			//Get a random rotation to spawn the pickup
			//This is not mandatory but makes it look nicer
			FRotator SpawnRotation;
			SpawnRotation.Yaw = FMath::FRandRange(1, 360);
			SpawnRotation.Pitch = FMath::FRandRange(1, 360);
			SpawnRotation.Roll = FMath::FRandRange(1, 360);
			//Spawn the pickup:
			World->SpawnActor<APickup>(WhatPickupToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			
			SpawnDelay = FMath::FRandRange(PickupSpawnDelayRangeLow, PickupSpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnPickup, SpawnDelay, false);
		}
	}
}

void ASpawnVolume::SpawnShots()
{
//If we set a projectile to shoot 
	if(WhatProjectileToSpawn)
	{
	//Check for valid world:
		UWorld* const World = GetWorld();
		if (World)
		{
		//Get the shot location and rotation
			FVector ShootOrigin = GetRandomPointInVolume();	
			//The rotation would be that of the Down Vector of the WhereToSpawn component
			FRotator ShootRotation = ShootDirection.Rotation();

			//Make the projectile go in the desired direction
			FVector ActualDirection = ShootOrigin + FTransform(ShootRotation).GetLocation(); 
			/** If you whant to randomize directions a bit put a couple rotation values here
			 * The Values should be clamped to avoid weirdness
			 * FRotator ActualRotation = ShootRotation;
			 * ActualRotation.Yaw += RandomYaw;
			 * ActualRotation.Pitch += RandomPitch;
			 * ActualRotation.Roll += RandomRoll;
			*/

		// Set the Shot Parameters
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
			spawnParams.Instigator = GetInstigator();
			//TO DO: For now it will spawn just one shot with low delay
			//Create an array of projectiles with their respective random spawn point each

		//Spawn the Shots
			ATankProjectile* Projectile = World->SpawnActor<ATankProjectile>(WhatProjectileToSpawn, ShootOrigin, ShootRotation, spawnParams);

			if (Projectile)
			{
				//Set the Projectile's initial trajectory
				Projectile->FireInDirection(ActualDirection);
			}
			ShootDelay = FMath::FRandRange(ProjectileDelayRangeLow, ProjectileDelayRangeHigh);
			GetWorldTimerManager().SetTimer(ShootingTimer, this, &ASpawnVolume::SpawnShots, ShootDelay, false);

		}
	}
}