// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "AIWeapon.h"
#include "Components/CapsuleComponent.h"
#include "LJMUBasic425GameModeBase.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Sets AIHealth as AIMaxHealth
	AIHealth = AIMaxHealth;

	//Spawns Weapon
	AIWeapon = GetWorld()->SpawnActor<AAIWeapon>(WeaponClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	AIWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_rSocket"));
	AIWeapon->SetOwner(this);

}

// Calls AIIsDead Animation
bool AAICharacter::AIIsDead() const
{
	return AIHealth <= 0;
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Applies damage
float AAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	//Applies Damage based on each weapon
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(AIHealth, DamageToApply);
	AIHealth -= DamageToApply;

	if(AIIsDead())
	{
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ALJMUBasic425GameModeBase* GameMode = GetWorld()->GetAuthGameMode<ALJMUBasic425GameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
	}
	
	return DamageToApply;
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Calls PullTrigger
void AAICharacter::AIShoot()
{
	AIWeapon->AIPullTrigger();
}