// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class AAIWeapon;

UCLASS()
class LJMUBASIC425_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION(BlueprintPure)
		bool AIIsDead() const;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	void AIShoot();

private:

	UPROPERTY(EditDefaultsOnly)
		float AIMaxHealth = 100;

	UPROPERTY(VisibleAnywhere)
		float AIHealth;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AAIWeapon> WeaponClass;

	UPROPERTY()
		AAIWeapon* AIWeapon;

};
