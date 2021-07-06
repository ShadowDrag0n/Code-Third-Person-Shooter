// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Weapon.h"
#include "Grenade.h"
#include "Components/CapsuleComponent.h"
#include "LJMUBasic425GameModeBase.h"
#include "WeaponArray.h"
#include "TimerManager.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Sets Health as MaxHealth
	Health = MaxHealth;

	// Spawns Weapon
	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	GetMesh()->HideBoneByName(TEXT("weapon"), EPhysBodyOp::PBO_None);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weaponSocket"));
	Weapon->SetOwner(this);

	// Spawns Grenade
	Grenade = GetWorld()->SpawnActor<AGrenade>(GrenadeClass);
	Grenade->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("GrenadeSocket"));
	Grenade->SetOwner(this);
	
}

// Calls IsDead Animation
bool APlayerCharacter::IsDead() const
{
	return Health <= 0;
}
// Gets the amount of Health left
float APlayerCharacter::GetHealthPercentage() const
{
	return Health / MaxHealth;
}

// Gets the amount of Ammo left
float APlayerCharacter::GetAmmoPercentage() const
{
	return Weapon->GetAmmo();
}

// Adds Health
void APlayerCharacter::Heal(float _HealAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("We are healing for %f points."), _HealAmount);
	Health += _HealAmount;

	if (Health > 100)
	{
		Health = 100;
	}
}

// Adds Ammo 
void APlayerCharacter::AddAmmo(int _AmmoAmount)
{
	Weapon->AddAmmo(_AmmoAmount);

	UE_LOG(LogTemp, Warning, TEXT("You have gained %f Ammo."), _AmmoAmount);
}

// Debug to check if healing works
//void APlayerCharacter::StartHealing()
//{
//	Heal(0.02f);
//}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::resetAnim()
{
	HasThrown = false;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Allows player to move forward and back
	// Allows player to look up and down
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);

	// Allows player to move right and left
	// Allows player to look right to left
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	// Allows the player to use their controller
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &APlayerCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &APlayerCharacter::LookRightRate);

	// Allows the player to Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	// Allows for player to heal using a button press
	//PlayerInputComponent->BindAction(TEXT("Heal"), IE_Pressed, this, &APlayerCharacter::StartHealing);

	PlayerInputComponent->BindAction(TEXT("Anim"), IE_Pressed, this, &APlayerCharacter::resetAnim);

	// Allows the player to Shoot
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Shoot);

	// Allows the player to throw a grenade
	PlayerInputComponent->BindAction(TEXT("Throw"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Throw_Implementation);

}

// Applies damage
float APlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// Applies Damage based on each weapon
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	
	// Debug code
	//UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);

	if (IsDead())
	{
		ALJMUBasic425GameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALJMUBasic425GameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	}
	
	return DamageToApply;
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

// Calls PullTrigger
void APlayerCharacter::Shoot()
{
	Weapon->PullTrigger();
}

void APlayerCharacter::Throw_Implementation()
{
		if (Grenade != nullptr)
		{
			HasThrown = true;
			UE_LOG(LogTemp, Warning, TEXT("Grenade 3 sec!"));
			GetWorldTimerManager().SetTimer(Timer, Grenade, &AGrenade::ThrowGrenade, ExplosionDelay, false);
			//GetWorldTimerManager().SetTimer(Timer2, this, &APlayerCharacter::FinishAnim, ExplosionDelay, false);

			//if (Grenade->ready)
			//{
			//	GetWorldTimerManager().SetTimer(Reset, Grenade, &AGrenade::ThrowGrenade, ResetDelay);
			//	HasThrown = false;
			//}

		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Grenade Not Thrown!"));
		}
}

void APlayerCharacter::FinishAnim()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop!"));
	HasThrown = false;
}