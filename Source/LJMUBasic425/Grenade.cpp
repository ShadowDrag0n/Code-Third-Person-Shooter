// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets up the root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Sets up the mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	ready = true;
}



void AGrenade::ThrowGrenade()
{
	// States player has thrown a grenade when trigger is pressed
	//UE_LOG(LogTemp, Warning, TEXT("Grenade Thrown!"));

	FHitResult GHit;
	FVector GrenadeDirection;

	// Sets bSuccess to call GrenadeTraceCode
	bool bSuccess = GrenadeTraceCode(GHit, GrenadeDirection);

	if(bSuccess)
	{
		ready = false;

		// Sets the Fvector and FRotator as Location and End
		FVector Location;
		FRotator Rotation;

		Location = this->GetActorLocation();
		Rotation = this->GetActorRotation();

		// Sets the Fvector as Start 
		FVector Start = GHit.Location;
		// Sets the Fvector as end 
		FVector End = GHit.Location + Rotation.Vector() * GrenadeRange;

		// Sets the array as GrenadeHits
		TArray<FHitResult> GrenadeHits;
		FVector StartTrace = Start;
		FVector EndTrace = End;

		// Sets the collision shape and name
		FCollisionShape CollisionShape;
		CollisionShape.ShapeType = ECollisionShape::Sphere;
		CollisionShape.SetSphere(Radius);
		
		// Ignores the Owner
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(GetOwner());

		// Creates Debug point at location
		//DrawDebugPoint(GetWorld(), GHit.Location,2, FColor::Blue, true);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GrenadeEffect, GHit.Location, GrenadeDirection.Rotation());

		bool Explosion = GetWorld()->SweepMultiByChannel(GrenadeHits, StartTrace, EndTrace, FQuat::FQuat(), ECollisionChannel::ECC_GameTraceChannel2, CollisionShape, Params);
		
		// Creates Debug Sphere at location
		//DrawDebugSphere(GetWorld(), StartTrace, Radius, 2, FColor::Red, true);

		if (Explosion)
		{

			for (auto Grenade = GrenadeHits.CreateIterator(); Grenade; Grenade++)
			{
				AActor* GHitActor = Grenade->GetActor();

				if (GHitActor != nullptr)
				{
					FPointDamageEvent DamageEvent(GrenadeDamage, *Grenade, GrenadeDirection, nullptr);
					AController* OwnerController = GetOwnerController();
					GHitActor->TakeDamage(GrenadeDamage, DamageEvent, OwnerController, this);
				}
			}

			ready = true;

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No Grenade left!"));
		}
		

	}

}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

bool AGrenade::GrenadeTraceCode(FHitResult& GHit, FVector& GrenadeDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;

	// Sets the Fvector and FRotator as Location and End
	FVector Location;
	FRotator Rotation;

	// Gets the camera location and rotation
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	
	// Sets the Fvector as end 
	FVector End = Location + Rotation.Vector() * GrenadeRange;

	GrenadeDirection = -Rotation.Vector();

	// Ignores the Owner
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(GHit, Location, End, ECollisionChannel::ECC_GameTraceChannel2, Params);
}

AController* AGrenade::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;

	return  OwnerPawn->GetController();
}





