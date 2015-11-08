// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LootableActorParent.generated.h"

UCLASS()
class RPGDEMO_API ALootableActorParent : public AActor
{
	GENERATED_BODY()

	public:	
		// Sets default values for this actor's properties
		ALootableActorParent();

		FTimerHandle m_AutoCloseHandle;

		FTimerHandle m_DestroyHandle;

		AActor* m_CurrentLooter;

		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Behavior)
		bool m_Opened;

		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Behavior)
		bool m_HasLoot = true;

		UPROPERTY(ReplicatedUsing = OnRep_LootChanged, EditAnywhere, BlueprintReadWrite, Category = Behavior)
		TArray<int32> m_Loot;

		UFUNCTION()
		void OnRep_LootChanged();

		void ToggleStatus(AActor* thePlayer);

		void AutoClose();

		void TriggerDestruction();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		// Called every frame
		virtual void Tick( float DeltaSeconds ) override;
	
};
