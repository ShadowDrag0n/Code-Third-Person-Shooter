// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Grenade.generated.h"

UCLASS()
class LJMUBASIC425_API AGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();

	void ThrowGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool ready;

private:

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		USoundBase* Impact;

	UPROPERTY(EditAnywhere)
		UParticleSystem* GrenadeEffect;

	UPROPERTY(EditAnywhere)
		float Radius = 500;

	UPROPERTY(EditAnywhere)
		float GrenadeRange = 1000;

	UPROPERTY(EditAnywhere)
		float GrenadeDamage = 10;

	bool GrenadeTraceCode(FHitResult& GHit, FVector& GrenadeDirection);

	AController* GetOwnerController() const;



};