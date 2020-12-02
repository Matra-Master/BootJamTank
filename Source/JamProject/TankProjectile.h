// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "Components/SphereComponent.h"
#include "TankProjectile.generated.h"

UCLASS()
class JAMPROJECT_API ATankProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Sphere collision radius
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float radius;

	//Projectile Initial speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float initialSpeed;
	
	//Projectile MAX speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float maxSpeed;

	//Projectile Bounciness or coefficient of restitution
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float bounciness;
	
	//Projectile gravity scale. 0.0f is no gravity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float projectileGravity;

	// Projectile mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	UStaticMeshComponent* ProjectileMeshComponent;
	
	//Projectile Material
	UPROPERTY(VisibleDefaultsOnly, Category = "Movement")
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	// Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComponent; 
	

	//Projectile Movement Component
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Function that initializes the projectile's velocity in the shoot direction
	void FireInDirection(const FVector& ShootDirection);
	
};
