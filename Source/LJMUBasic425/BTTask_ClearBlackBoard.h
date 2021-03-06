// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearBlackBoard.generated.h"

/**
 * 
 */
UCLASS()
class LJMUBASIC425_API UBTTask_ClearBlackBoard : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UBTTask_ClearBlackBoard();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
