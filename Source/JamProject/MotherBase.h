// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MotherBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JAMPROJECT_API UMotherBase : public USceneComponent
{
	GENERATED_BODY()
	
	
	//The zone considered as Base
	UPROPERTY(Category = "Zone", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* zone;
	
	//A box to animate and show when the tank leaves some pickups
	UPROPERTY(Category = "Zone", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* delivery;
	
	//Here goes the list of actors that are in the Base zone
	UPROPERTY(Category = "Zone", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ActorsNearby;

public:	
	// Sets default values for this component's properties
	UMotherBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Checks if a tank is at the base */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "zone")
	bool bTankInBase;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

	/** Returns the zone sphere subobject */
	FORCEINLINE class USphereComponent* GetZone() const { return zone; }
		
	/** Returns the delivered subobject */
	FORCEINLINE class UStaticMeshComponent* GetDelivery() const { return delivery; }
	
	/** Returns the bTankInBase bool */
	FORCEINLINE bool GetTankInBase() const { return bTankInBase; }

	/** Check any actor in the base zone to know if theres a tank among them
	 * Works with only THE tank because this is single player for now */
	UFUNCTION(BlueprintCallable, Category = "Zone")
	void CheckTankInBase();
};
