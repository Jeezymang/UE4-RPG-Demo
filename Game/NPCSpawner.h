// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "NPCSpawner.generated.h"

UCLASS()
class RPGDEMO_API ANPCSpawner : public AActor
{
	GENERATED_BODY()
	
	public:	
		// Sets default values for this actor's properties
		ANPCSpawner();

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		int32 m_SpawnInterval;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		int32 m_NPCLimit;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		TSubclassOf<AActor> m_NPCClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		float m_SpawnChance;

		FTimerHandle m_SpawnDelegate;

		void SpawnTrigger();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		// Called every frame
		virtual void Tick( float DeltaSeconds ) override;
};
