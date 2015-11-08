// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "BotAIController.h"
#include "Bots/BotCharacterParent.h"


bool ABotAIController::GetRandomPointInRadius(UNavigationSystem* navSys, FVector origin, float radius, FVector& location)
{
	FNavLocation resultLocation;
	bool wasSuccess = navSys->GetRandomPointInNavigableRadius(origin, radius, resultLocation);
	location = resultLocation.Location;
	return wasSuccess;
}
