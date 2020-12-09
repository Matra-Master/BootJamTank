// Fill out your copyright notice in the Description page of Project Settings.


#include "TankProjectile.h"


// Sets default values
ATankProjectile::ATankProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Initial values
	radius = 5.0f;
	initialSpeed = 500.0f;
	maxSpeed = 500.0f;
	bounciness = 0.3f;
	projectileGravity = 0.0f;

	if(!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
	if(!CollisionComponent)
	{
		//Use a sphere as a simple collision representation
		//If you have the time make it a tank projectile shape, just for the fun
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		//Set the default sphere's collision radius
		CollisionComponent->InitSphereRadius(radius);
		//Set root component to be the collision component
		//I don't exactly understand why generate rootcomponent with that "if" above
		//and then assign it again here
		//But doubts will wait. Besides its in an unreal documentation tutorial so f*ck it
		RootComponent = CollisionComponent;
	}
	if(!ProjectileMovementComponent)
	{
		//Use this component to drive this projectile's movement
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PojectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = initialSpeed;
		ProjectileMovementComponent->MaxSpeed = maxSpeed;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = bounciness;
		ProjectileMovementComponent->ProjectileGravityScale = projectileGravity;
	}

	if(!ProjectileMeshComponent)
	{
	    ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	    static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Sphere.Sphere'"));
	    if(Mesh.Succeeded())
	    {
	        ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
	    }
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/SphereMaterial.SphereMaterial'"));
	if(Material.Succeeded())
	{
		ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ATankProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//Function that initializes the projectile's velocity in the shoot direction
void ATankProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}