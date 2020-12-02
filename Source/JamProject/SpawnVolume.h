// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SpawnVolume.generated.h"

UCLASS()
class JAMPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//The Pickup to Spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class APickup> WhatPickupToSpawn;

	//The projectile to Spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class ATankProjectile> WhatProjectileToSpawn;

	
	//FTimer cant be UPROPERTYs O.o
	FTimerHandle SpawnTimer;
	FTimerHandle ShootingTimer;
	
	//Minimum spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float PickupSpawnDelayRangeLow;
	//Maximum spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float PickupSpawnDelayRangeHigh;

	//Minimum spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float ProjectileDelayRangeLow;
	//Maximum spawn delay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float ProjectileDelayRangeHigh;
	
	/** Number of projectiles to spawn at the same time
	 * I want to be able to increase it with difficulty 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	uint8 ProjectileShootingNumber;
	*/
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//Find a random point within the box component
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetRandomPointInVolume();
	
	//Returns the WhereToSpawn subobject
	class UBoxComponent* GetWhereToSpawn();
	
	/** Funtion to set the Shoot Direction */
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SetShootDirection(FVector NewDirection);

	/** Get the shoot direction vector */
	UFUNCTION(BlueprintPure, Category = "Spawning")
	FVector GetShootDirection();
	
private:
	/** Box Component that specify where to spawn stuff */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	UBoxComponent* WhereToSpawn;

	/** Vector that signals the direction to shoot spawned stuff 
	 * Useful for spawning projectiles through various SpawnVolumes in a world
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = true))
	FVector ShootDirection;

	//Handle spawning a new pickup
	 void SpawnPickup();
	 
	 /** Handle spawning projectiles
	  * It will always shoot in the direction of the variable ShootDirection
	  * TODO: make the direction a cone and spawn in a random vector from thacone.
	 */
	 void SpawnShots ();
	 
	 //The Current spawn delay
	 float SpawnDelay;
	//The current shooting delay
	 float ShootDelay;
};
