// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "StaticPlayerAbilities.generated.h"

class ARPGDemoCharacter;

UCLASS()
class RPGDEMO_API UStaticPlayerAbilities : public UObject
{
	GENERATED_BODY()


	public:

		static void AttemptHeavyAttack(AActor* theActor);
		static void AttemptKnockout(AActor* theActor);
		static void AttemptWeakPushback(AActor* theActor);
		static void AttemptWeakLifeDrain(AActor* theActor);
		static void AttemptStrikedown(AActor* theActor);
		static void InflictDamageOnTarget(AActor* theActor, int32 damageAmount);
};
