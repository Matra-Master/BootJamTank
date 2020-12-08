// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Pickup.h"
#include "CratePickup.h"


// Sets default values
ATank::ATank()
{
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create the simple root point
	rootPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootPoint"));
	RootComponent = rootPoint;
	
	/** 
	 * TRAIN COMPONENT GROUP
	*/
	//HITBOX
	hitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	/* Setup Attachment is a function better used in a constructor.
	To change attachments or reattach something outside of this function
	it's best to use the AttachToComponent function.
	See Switch for AttachToComponent usage.*/
	hitBox ->SetupAttachment(RootComponent);
	//TRAIN
	train = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Train"));
	train->SetupAttachment(hitBox);
	//CARGO
	cargo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cargo"));
	cargo->SetupAttachment(hitBox);
	
	/**
	 * TURRET COMPONENT GROUP
	*/
	//ARROW
	arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrow->SetupAttachment(RootComponent);
	//TURRET 
	turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	turret->SetupAttachment(arrow);
	//CAMERA ARM
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
	//CAMERA
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cam->SetupAttachment(arm, USpringArmComponent::SocketName);

	//PICKUPSPHERE CONFIGS
	//Create the sphere that collects close pickups
	pickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	pickupSphere->SetupAttachment(RootComponent);
	pickupSphere->SetSphereRadius(200.f);
	//GAMEPLAY INITIALIZATIONS

	//With this my character should rotate with the camera
	bUseControllerRotationYaw = true;
	//The initial state of the player camera perspective
	firstPerson = false;
	springArmMaxPitchCap = 25;
	springArmMinPitchCap = -45;
	springArmMaxYawCap = 25;
	springArmMinYawCap = -45;

	//Movement 
	movementLength = 20.f;
	rotationSpeed = 20.f;
	
	//Firing
	muzzleOffset = FVector(90.f, 0.f, 0.f);
	fireRate = 0.5f;
	bCanFire = true;
	
	//Scoring
	TankScore = 0.f;
	Combo = 0.f;
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
	//Instantiate the pickup hidden
	if(cargo)
	{
		cargo->SetHiddenInGame(true);
	}
	Combo = 0.f;
	
}

//DONE
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

//DONE
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

//DONE
void ATank::Switch()	
{
	
	if(firstPerson)
	{
		arm->TargetArmLength = 500.0f;
		arm->SetRelativeRotation(FRotator(-45, 0, 0));
		cam->SetRelativeRotation(FRotator(0, 0, 0));
		/* AttachToComponent needs an extra argument respecto to the classic AttachTo.
		It's a class of transform rules. I used a const called KeepRelativeTransform to maintain my camera as is. 
		*/
		cam->AttachToComponent(arm, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
		firstPerson = false;
	}
	else
	{
		cam->AttachToComponent(arrow, FAttachmentTransformRules::KeepRelativeTransform);
		firstPerson = true;
	}
}

//DONE
void ATank::HorizontalRot(float value)
{
	if (value)
	{
		arrow->AddLocalRotation(FRotator(0, value, 0));
	}
}
//DONE
void ATank::VerticalRot(float value)
{
	if (value)
	{
		float temp = 0;
		if(!firstPerson)
		{
			temp = arm->GetRelativeRotation().Pitch + value;
			//Temp exist to be compared with the cap values
			//That way my camera wont go further than some vertical values
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
//DONE
void ATank::MoveForward(float value)
{
	if (value)
	{
		const FVector MoveDirection = hitBox->GetForwardVector();
		const FVector Movement = MoveDirection * value * movementLength;  
		
		const FRotator Rotation = MoveDirection.Rotation();
		RootComponent->MoveComponent(Movement, Rotation, true);
	}
}
//DONE
void ATank::MoveRight(float value)
{
	if(value)
	{
		float temp = value * rotationSpeed;
		hitBox->AddRelativeRotation(FRotator(0, temp, 0));
	}
}

//DONE
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

//DONE
void ATank::CollectPickups()
{
	//Get all overlapping actors and store them in an array
	TArray<AActor*> CollectedActors;
	pickupSphere->GetOverlappingActors(CollectedActors);

	//For each actor collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		//Cast the actor to a Crate pickup
		ACratePickup* const TestPickup = Cast<ACratePickup>(CollectedActors[iCollected]);

		//If the cast is succesful and the pickup is valid and active
		if (TestPickup && !TestPickup->IsPendingKill() && TestPickup->IsActive())
		{
			//Load your score
			TankScore += TestPickup->GetScore();
			//Combo+
			ComboPlus();
			//Call the pickup's WasCollected Function
			TestPickup->WasCollected();
			//Deactivate the pickup
			TestPickup->SetActive(false);
		}
	}
	//If you collected pickups then show it
	if(TankScore > 0)
	{
		//Reveal a mesh of some box behind your tank
		cargo->SetHiddenInGame(false);
		//NOTIFY in case you are at the safe zone to unload your cargo
		
		FString someEvent = "COLLECTED";

		//NOTIFY COLLECTED

		//Exit immediatly, the gamemode will call an unload function
	}
}

//DONE
void ATank::UnloadCargo()
{
	TankScore = 0;
	Combo = 0;
	cargo->SetHiddenInGame(false);
}

//            UNDONE
//This could be a bigger funcion with more than one observer class in play
void ATank::notify(FString event)
{
	//SHould be event based, unreal can do it
		
}

//DONE
// Set the tank score value
void ATank::SetTankScore(float score)
{
	TankScore = score;
}

void ATank::ComboPlus()
{
    Combo += 1;
}

void ATank::ComboReset()
{
    Combo = 0;
}
