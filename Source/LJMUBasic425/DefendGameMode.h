// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LJMUBasic425GameModeBase.h"
#include "DefendGameMode.generated.h"

/**
 * 
 */
UCLASS()
class LJMUBASIC425_API ADefendGameMode : public ALJMUBasic425GameModeBase
{
	GENERATED_BODY()

public:

	virtual void PawnKilled(APawn* PawnKilled) override;

private:

	void EndGame(bool bIsPlayerWinner);
	
};
