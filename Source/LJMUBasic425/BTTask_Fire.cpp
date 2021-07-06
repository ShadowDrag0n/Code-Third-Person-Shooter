// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Fire.h"
#include "AICharacter.h"
#include "AIController.h"

UBTTask_Fire::UBTTask_Fire()
{
	NodeName = "AIShoot";
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AAICharacter* Character = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	Character->AIShoot();

	return EBTNodeResult::Succeeded;
}