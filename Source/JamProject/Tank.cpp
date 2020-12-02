// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Pickup.h"


// Sets default values
ATank::ATank()
{
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create the tank's Hit Box for projectile colisions
	hitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	RootComponent = hitBox;
	//Create the tank's moving train
	train = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Train"));
	train->SetupAttachment(RootComponent);
	
	//Create the super important arrow
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow->SetupAttachment(RootComponent);

	//PICKUPSPHERE CONFIGS
	//Create the sphere that collects close pickups
	pickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	pickupSphere->SetupAttachment(RootComponent);
	pickupSphere->SetSphereRadius(200.f);
	
	//TURRET CONFIGS
	//Create the tank firing turret
	turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	turret->SetupAttachment(arrow);

	
	//With this my character should rotate with the camera
	bUseControllerRotationYaw = true;

	//Create a spring arm for the camera
	arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	arm->SetupAttachment(arrow);
	arm->TargetArmLength = 500.f;
	arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	arm->bDoCollisionTest = false; //Dont pull the cammera when colliding with something
	//CameraLag options just in case
	arm->bEnableCameraLag = true;
	arm->CameraLagSpeed = 2;
	arm->CameraLagMaxDistance = 1.0f;
	arm->bEnableCameraRotationLag = true;
	arm->CameraRotationLagSpeed = 4;
	arm->CameraLagMaxTimeStep = 1;

	//Create a Camera
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//Attach camera to the end of the arm
	cam->SetupAttachment(arm, USpringArmComponent::SocketName);

	//GAMEPLAY INITIALIZATIONS

	//The initial state of the player camera perspective
	firstPerson = false;
	springArmMaxPitchCap = 25;
	springArmMinPitchCap = -45;
	springArmMaxYawCap = 25;
	springArmMinYawCap = -45;

	//Movement Length
	movementLength = 20.f;
	
	rotationSpeed = 20.f;
	
	muzzleOffset = FVector(90.f, 0.f, 0.f);
	fireRate = 0.5f;
	bCanFire = true;
	
	TankScore = 0.f;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	//Instantiate the pickup hidden
	if(pickedup)
	{
		pickedup->SetActorHiddenInGame(true);
	}
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Forward", this, &ATank::MoveForward);
	InputComponent->BindAxis("Horizontal", this, &ATank::MoveRight);
	InputComponent->BindAxis("HorizontalRot", this, &ATank::HorizontalRot);
	InputComponent->BindAxis("VerticalRot", this, &ATank::VerticalRot);
	InputComponent->BindAxis("Zoom", this, &ATank::Zoom);

	InputComponent->BindAction("ChangeCamera", IE_Pressed, this, &ATank::Switch);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATank::Fire);
	InputComponent->BindAction("Collect", IE_Pressed, this, &ATank::CollectPickups);
}


void ATank::Zoom(float value)
{
	if(value)
	{
		float temp = arm->TargetArmLength + (value * -10);
		if (temp < 1000 && temp > 250)
		{
			arm->TargetArmLength = temp;
		}
	}
}

void ATank::Switch()	
{
	
	if(firstPerson)
	{
		arm->TargetArmLength = 500.0f;
		arm->SetRelativeRotation(FRotator(-45, 0, 0));
		cam->SetRelativeRotation(FRotator(0, 0, 0));
		cam->AttachToComponent(arm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
		firstPerson = false;
	}
	else
	{
		cam->AttachToComponent(arrow, FAttachmentTransformRules::KeepRelativeTransform);
		firstPerson = true;
	}
}

void ATank::HorizontalRot(float value)
{
	if (value)
	{
		arrow->AddLocalRotation(FRotator(0, value, 0));
	}
}

void ATank::VerticalRot(float value)
{
	if (value)
	{
		float temp = 0;
		if(!firstPerson)
		{
			temp = arm->GetRelativeRotation().Pitch + value;
			if (temp < springArmMaxPitchCap && temp > springArmMinPitchCap)
			{
				arm->AddLocalRotation(FRotator(value, 0, 0));
			}	
		}
		else
		{
			//temp = cam->GetRelativeRotation().Pitch + value;
			//if(temp < 65 && temp > -65)
			//{
				cam->AddLocalRotation(FRotator(value, 0, 0));
			//}
		}
	}
}

void ATank::MoveForward(float value)
{
	if (value)
	{
		const FVector MoveDirection = RootComponent->GetForwardVector();
		const FVector Movement = MoveDirection * value * movementLength;  
		
		const FRotator Rotation = MoveDirection.Rotation();
		RootComponent->MoveComponent(Movement, Rotation, true);
	}
}

void ATank::MoveRight(float value)
{
	if(value)
	{
		float temp = value * rotationSpeed;
		RootComponent->AddRelativeRotation(FRotator(0, temp, 0));
	}
}

//Fire Projectiles in the direction of the frontal arrow
void ATank::Fire()
{
	//Attempt to fire a projectile	
	if(projectileClass && bCanFire)
	{
		//Get the arrow location and rotation
		FVector arrowLocation = arrow->GetComponentLocation();
		FRotator arrowRotation = arrow->GetComponentRotation();


		muzzleOffset.Set(200.0f, 0.0f, 0.0f);
		

		//Transform muzzleOffset from arrow space to world space
		FVector muzzleLocation = arrowLocation + FTransform(arrowRotation).TransformVector(muzzleOffset);


		FRotator muzzleRotation = arrowRotation;
		//muzzleRotation.Pitch += 10.0f;

		//THE World
		UWorld* World = GetWorld();
		if(World)
		{
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = this;
		    spawnParams.Instigator = GetInstigator();
		    // Spawn the projectile at the muzzle.
		    ATankProjectile* Projectile = World->SpawnActor<ATankProjectile>(projectileClass, muzzleLocation, muzzleRotation, spawnParams);
		    if (Projectile)
		    {
		        // Set the projectile's initial trajectory.
		        FVector LaunchDirection = muzzleRotation.Vector();
		        Projectile->FireInDirection(LaunchDirection);
		    }
		}
	}
}

void ATank::CollectPickups()
{
	//Get all overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	pickupSphere->GetOverlappingActors(CollectedActors);
	//For each actor collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		//Cast the actor to a pickup
		APickup* const TestPickup = Cast<APickup>(CollectedActors[iCollected]);
		//If the cast is succesful and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			//Call the pickup's Wascollected Function
			TestPickup->WasCollected();
			//Deactivate the pickup
			TestPickup->SetActive(false);
			
			TankScore += TestPickup->GetScore();
		}
	}
	//If you collected pickups then show it
	if(TankScore > 0)
	{
		//Reveal a mesh of some box behind your tank
		cargo->SetActorHiddenInGame(false);
	}
	//Notify in case you are at the safe zone to unload your cargo
	
	//Exit immediatly, the gamemode will call an unload function
}

void ATank::UnloadCargo()
{
	TankScore = 0;
	cargo->SetActorHiddenInGame(true);
}

// Set the tank score value
void ATank::SetTankScore(float score)
{
	TankScore = score;
}