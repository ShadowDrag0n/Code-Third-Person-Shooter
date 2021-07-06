// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIWeapon.generated.h"

UCLASS()
class LJMUBASIC425_API AAIWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIWeapon();

	void AIPullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* AIRoot;

	UPROPERTY(VisibleAnywhere)
		USkeletalMeshComponent* AIMesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* AIMuzzleFlash;

	UPROPERTY(EditAnywhere)
		USoundBase* AIMuzzleSoundEffect;

	UPROPERTY(EditAnywhere)
		USoundBase* AIImpact;

	UPROPERTY(EditAnywhere)
		UParticleSystem* AIImpactEffect;

	UPROPERTY(EditAnywhere)
		float AIMaxRange = 5000;

	UPROPERTY(EditAnywhere)
		float AIDamage = 10;

	bool AIWeaponTraceCode(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;
};
