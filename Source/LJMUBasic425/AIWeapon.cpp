// Fill out your copyright notice in the Description page of Project Settings.


#include "AIWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
AAIWeapon::AAIWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//sets up the root
	AIRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(AIRoot);

	//sets up the mesh
	AIMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	AIMesh->SetupAttachment(AIRoot);

}

void AAIWeapon::AIPullTrigger()
{
	
	//sets the emitter for the weapon
	UGameplayStatics::SpawnEmitterAttached(AIMuzzleFlash, AIMesh, TEXT("MuzzleFlashSocket"));

	//sets the sound attached to the weapon
	UGameplayStatics::SpawnSoundAttached(AIMuzzleSoundEffect, AIMesh, TEXT("MuzzleFlashSocket"));


	//sets hit and shot direction
	FHitResult Hit;
	FVector ShotDirection;

	//sets bSuccess to call WeaponTraceCode
	bool bSuccess = AIWeaponTraceCode(Hit, ShotDirection);

	if (bSuccess)
	{

		//Spawns ImpactEffect at Location
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AIImpactEffect, Hit.Location, ShotDirection.Rotation());

		//Spawns Impact sound at location
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), AIImpact, Hit.Location);

		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(AIDamage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(AIDamage, DamageEvent, OwnerController, this);
		}
	}
}
// Called when the game starts or when spawned
void AAIWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAIWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AAIWeapon::AIWeaponTraceCode(FHitResult& Hit, FVector& ShotDirection)
{
	
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) 
		return false;

	//Sets Vector and Rotation
	FVector Location;
	FRotator Rotation;

	//Gets player camera location and rotation
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	ShotDirection = -Rotation.Vector();

	//Finds the end point
	FVector End = Location + Rotation.Vector() * AIMaxRange;

	//Ignores Owner
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());

	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

}

AController* AAIWeapon::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) 
		return nullptr;
	
	return  OwnerPawn->GetController();

}

