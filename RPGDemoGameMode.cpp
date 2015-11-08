// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "RPGDemo.h"
#include "RPGDemoGameMode.h"
#include "RPGDemoCharacter.h"
#include "Game/CustomPlayerHUD.h"

ARPGDemoGameMode::ARPGDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	HUDClass = ACustomPlayerHUD::StaticClass();
}

void ARPGDemoGameMode::BeginPlay()
{
	if (Role == ROLE_Authority)
		UStaticFunctions::InitializeData();
}

void ARPGDemoGameMode::PostLogin(APlayerController* newPlayer)
{
	Super::PostLogin(newPlayer);
	APawn* thePawn = newPlayer->GetPawn();
	if (!thePawn) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(thePawn);
	if (!thePlayer) return;
	GetWorld()->GetTimerManager().SetTimer(thePlayer->m_EnergyRegenHandle, thePlayer, &ARPGDemoCharacter::AttemptEnergyRegen, thePlayer->m_EnergyRegenInterval, true);
	thePlayer->m_Energy = thePlayer->m_MaxEnergy;
	thePlayer->m_BodyColor = FLinearColor(FMath::FRandRange(0.3, 0.8), FMath::FRandRange(0.3, 0.8), FMath::FRandRange(0.3, 0.8), 1);
	thePlayer->GiveQuest("Kill Murlocs");
}