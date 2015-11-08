// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BotAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class RPGDEMO_API ABotAIController : public AAIController
{
	GENERATED_BODY()

	public:

		bool GetRandomPointInRadius(UNavigationSystem* navSys, FVector origin, float radius, FVector& location);
};
