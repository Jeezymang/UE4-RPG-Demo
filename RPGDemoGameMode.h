// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "RPGDemoGameMode.generated.h"

UCLASS(minimalapi)
class ARPGDemoGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ARPGDemoGameMode();

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* newPlayer) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TArray<FEquipmentBlueprint> m_EquipmentBlueprints;
};



