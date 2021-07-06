// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControl.generated.h"

/**
 * 
 */
UCLASS()
class LJMUBASIC425_API AAIControl : public AAIController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	bool AIIsDead() const;
	
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
		class UBehaviorTree* AIBehavior;
};
