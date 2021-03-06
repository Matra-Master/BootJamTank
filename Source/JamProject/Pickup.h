// Fill out your copyright notice in the Description page of Project Settings.

/** Pickup is a template for any other pickup i want to do.
 * It should tell if its active or not for beign pickable.
 * It will also have a inherent score value for gameplay purposes
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Pickup.generated.h"

UCLASS()
class JAMPROJECT_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//True when the pickup is pickable
	bool bIsActive;
	
	/** scoring value for any Pickup */
	UPROPERTY(Category = "Score", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float score;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Function to get the value of IsActive
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	//Procedure to set the value of IsActive
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool PickupState);
	
	// Static mesh to represent the pickup in a level 
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Pickup", meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditAnywhere, Category = "Pickup")
	class UStaticMeshComponent* PickupMesh;

	//Function to call when the pickup is collected
	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation();

	//FORCEINLINE class UStaicMeshComponent* GetMesh() const {return PickupMesh; }

	/** Sets the score value for this pickup */
	UFUNCTION(BlueprintCallable, Category = "Score")
	void SetScore(float Number);

	/** Get the score value for this pickup */
	UFUNCTION(BlueprintPure, Category = "Score")
	float GetScore() const;

private:

};
