// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MotherBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JAMPROJECT_API UMotherBase : public USceneComponent
{
	GENERATED_BODY()
	
	
	UPROPERTY(Category = "Zone", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* zone;
	
	UPROPERTY(Category = "Zone", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* delivery;
	
	UPROPERTY(Category = "Zone", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class TArray<AActor*> ActorsNearby;

public:	
	// Sets default values for this component's properties
	UMotherBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Checks if the tank is at the base */
	UPROPERTY(Blueprint, Category = "zone")
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

	UPROPERTY(BlueprintCallable, Category = "Zone")
	void CheckTankInBase();
};
