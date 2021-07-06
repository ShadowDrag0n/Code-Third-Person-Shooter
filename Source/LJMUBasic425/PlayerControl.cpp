// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControl.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

void APlayerControl::BeginPlay()
{
	Super::BeginPlay();

	// Allows for the HUD to appear during gameplay
	HUD = CreateWidget(this, HUDClass);
	if (HUD != nullptr)
	{
		HUD->AddToViewport();
	}
}

void APlayerControl::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	// Debug code
	//UE_LOG(LogTemp, Warning, TEXT("Finished"));

	HUD->RemoveFromViewport();
	
	if(bIsWinner)
	{
		// Allows the win screen to appear when player dies
		UUserWidget* WinScreen = CreateWidget(this, WinScreenClass);
		if (WinScreen != nullptr)
		{
			WinScreen->AddToViewport();
		}
	}
	else
	{
		// Allows the lose screen to appear when player dies
		UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
		if(LoseScreen != nullptr)
		{
			LoseScreen->AddToViewport();
		}	
	}
	
	GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
}


