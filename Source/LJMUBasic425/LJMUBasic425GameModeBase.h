// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LJMUBasic425GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LJMUBASIC425_API ALJMUBasic425GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	virtual void PawnKilled(APawn* PawnKilled);
};
