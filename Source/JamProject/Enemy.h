// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankProjectile.h"
#include "Enemy.generated.h"

UCLASS()
class JAMPROJECT_API AEnemy : public APawn
{
	GENERATED_BODY()
		
	/** Enemy should be a mesh and an arrow
	 * The arrow should follow the player or its general direction
	 * It should shoot projectiles at a rate based on the game mode timers
	 * 
	 */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* body;

	/** Arrow initialization */
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* arrow;

public:
	// Sets default values for this pawn's properties
	AEnemy();
	
	// Function that handles firing projectiles
	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class ATankProjectile> projectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Can this actor shoot now?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
	bool bCanFire;
	
	
	//GET AND SET FUNCTIONS
	/** Returns the Body mesh subobject */
	FORCEINLINE class UStaticMeshComponent* GetBody() const { return body; }
	
	/** Returns the Arrow subobject */
	FORCEINLINE class UArrowComponent* GetArrow() const { return arrow; }
	/**  */
};
