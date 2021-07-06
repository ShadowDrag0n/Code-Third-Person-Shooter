// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControl.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "AICharacter.h"


void AAIControl::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		RunBehaviorTree(AIBehavior);
		
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()); 

		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());

	}
	
}

void AAIControl::Tick(float DeltaSeconds)
{

	Super::Tick(DeltaSeconds);

}

bool AAIControl::AIIsDead() const
{
	AAICharacter* ControlledCharacter = Cast<AAICharacter>(GetPawn());
	if(ControlledCharacter != nullptr)
	{
		return ControlledCharacter->AIIsDead();
	}

	return true;
}
