// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"

#include "Blueprint/UserWidget.h"

#include "TankProjectile.h"

#include "Tank.generated.h"

UCLASS()
class JAMPROJECT_API ATank : public APawn
{
	GENERATED_BODY()
	
	
	/** This simple invisible point is my answer to the tank movement problems */
	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* rootPoint;
	/** Train Mesh
	 * It should always follow the movement direction
	 */
	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* train;
	/** Turret Mesh
	 * It should always follow the camera
	 */
	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* turret;
	//The Camera attached to an arm
	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* cam;
	//Spring arm for the camera, attached to the turret
	UPROPERTY(Category = "Camera", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* arm;
	//Tank Hit Box for damage calculations
	UPROPERTY(Category = "Areas", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* hitBox;
	//Sphere for Pickup collection
	UPROPERTY(Category = "Areas", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* pickupSphere;
	//Arrow that shows wich way the tank is facing	
	UPROPERTY(Category = "Areas", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* arrow;
	/** Is the tank at base? */
	UPROPERTY(Category = "Areas", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsAtBase;

	/** Pickup mesh
	 * It should be visible when you interact with a pickup near you
	 */
	UPROPERTY(Category = "Mesh", VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* cargo;
	
	/** Tank Score to be delivered at the safe point */
	UPROPERTY(Category = "Scoring", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TankScore;

	/** Tank Combo number */
	UPROPERTY(Category = "Scoring", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Combo;

public:
	// Sets default values for this pawn's properties
	ATank();

	// Function that handles firing projectiles
	void Fire();

	// Movement should be gridlike, so only forward or only horiontal
	void MoveForward(float value);	
	void MoveRight(float value);	
	/** Horizontal rotation for the spring arm or the camera based on the perspective */
	void HorizontalRot(float value);
	/** Vertical rotation for first person perspective */
	void VerticalRot(float value);

	/** Changes proximity of camera in third person */
	void Zoom(float value);
	/** Switches camera from third person to first person */
	void Switch();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Projectile class to spawn
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class ATankProjectile> projectileClass;

	/** Called to collect any pickup object that's inside the range of the Tank
	 * It updates the combo counter for every thing you pick up and also
	 * adds to the tank score */
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();
	
	/** Checks if we are at base */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	bool CheckIfAtBase();
	
	//Called to notify gamemode about an event
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void notify(FString event);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//CAMERA PROPPERTIES
	//Caps for the spring arm movements
	UPROPERTY(EditAnywhere, Category = "Cameras")
	float springArmMaxPitchCap;
	UPROPERTY(EditAnywhere, Category = "Cameras")
	float springArmMinPitchCap;
	UPROPERTY(EditAnywhere, Category = "Cameras")
	float springArmMaxYawCap;
	UPROPERTY(EditAnywhere, Category = "Cameras")
	float springArmMinYawCap;

	// Is the camera in first person mode?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cameras") 
	bool firstPerson;

	//GAMEPLAY STUFF
	//
	// Muzzle offset from the arrow location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector muzzleOffset;

	//The "Stride" the tank makes every time you press a move button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float movementLength;
	
	//The speed of the tank rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float rotationSpeed;

	//The fire rate for the cannon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float fireRate;
	
	//Is it safe to fire now?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool bCanFire;
	
	
	//GET AND SET FUNCTIONS
	/** Returns the Train mesh subobject */	
	FORCEINLINE class UStaticMeshComponent* GetTrain() const { return train; }
	/** Returns the Turret mesh subobject */	
	FORCEINLINE class UStaticMeshComponent* GetTurret() const { return turret; }
	/** Returns the Camera subobject */
	FORCEINLINE class UCameraComponent* GetCamera() const {return cam; }
	/** Returns the spring arm subobject */
	FORCEINLINE class USpringArmComponent* GetSpringArm() const { return arm; }
	/** Returns the Hit Box subobject */
	FORCEINLINE class UBoxComponent* GetHitBox() const { return hitBox; }
	/** Returns the Pickup Sphere subobject */
	FORCEINLINE class USphereComponent* GetPickupSphere() const { return pickupSphere; }
	

	/** Returns the Tank Score number */
	FORCEINLINE float GetTankScore() const { return TankScore; }
	/** Set the tank own score */
	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void SetTankScore(float score);
	/** Returns the tank combo number */
	FORCEINLINE float GetCombo() const { return Combo; }
	/** Combo + 1 */
	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void ComboPlus();
	/** Combo reset */
	UFUNCTION(BlueprintCallable, Category = "Scoring")
	void ComboReset();

	//Called to unload cargo at a nearby safe zone
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void UnloadCargo();
};
