// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Containers/Array.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Sets up the root
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Sets up the mesh
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

float AWeapon::GetAmmo() const
{
	return (float) Ammo / MaxAmmo;
}

void AWeapon::PullTrigger()
{
	// States player has been shot when trigger is pressed
	//UE_LOG(LogTemp, Warning, TEXT("You've been shot!"));

	// Sets hit and shot direction
	FHitResult Hit;
	FVector ShotDirection;

	// Sets the bullet per shot
	int BulletPerShot = 1;
	
	
	if (Ammo <= 0)
	{
		// Spawns Empty sound for weapon
		UGameplayStatics::SpawnSoundAttached(MuzzleEmptyEffect, Mesh, TEXT("MuzzleFlashSocket"));
	}
	else
	{
		// Decreases amount of ammo per shot
		Ammo -= BulletPerShot;

		// Sets bSuccess to call WeaponTraceCode
		bool bSuccess = WeaponTraceCode(Hit, ShotDirection);

		// Sets the emitter for the weapon
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

		// Sets the sound attached to the weapon
		UGameplayStatics::SpawnSoundAttached(MuzzleSoundEffect, Mesh, TEXT("MuzzleFlashSocket"));
		
		if (bSuccess)
		{
			// Spawns ImpactEffect at Location
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());

			// Spawns Impact sound at Location
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Impact, Hit.Location);

			// Applies emitter and damage
			AActor* HitActor = Hit.GetActor();

			if (HitActor != nullptr)
			{
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				AController* OwnerController = GetOwnerController();
				HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}


		}
		
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	Ammo = MaxAmmo;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::AddAmmo(int _AmmoAmount)
{
	Ammo += _AmmoAmount;

	if (Ammo > 100)
	{
		Ammo = 100;
	}
}


// Called for line trace
bool AWeapon::WeaponTraceCode(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
		return false;
	
	// Sets Vector and Rotation
	FVector Location;
	FRotator Rotation;

	// Gets player camera location and rotation
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	ShotDirection = -Rotation.Vector();

	// Finds the end point
	FVector End = Location + Rotation.Vector() * MaxRange;

	// Ignores Owner
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());


	return  GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);

}

AController* AWeapon::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) 
		return nullptr;
	
	return  OwnerPawn->GetController();

}
