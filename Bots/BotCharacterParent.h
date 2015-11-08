// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Libraries/StaticEnums.h"
#include "Libraries/StaticStructs.h"
#include "BotCharacterParent.generated.h"

class ABotAIController;
class ALootableActorParent;

UCLASS()
class RPGDEMO_API ABotCharacterParent : public ACharacter
{
	GENERATED_BODY()

		UMaterialInstanceDynamic* m_BodyMaterialInstance;

	public:
		// Sets default values for this character's properties
		ABotCharacterParent();

		ABotAIController* m_Controller;

		UNavigationSystem* m_NavSystem;

		AActor* m_Target;

		AActor* m_Killer;

		FVector m_CurrentWaypoint;

		FTimerHandle m_PatrolIdleHandle;

		FTimerHandle m_TargetSearchHandle;

		FTimerHandle m_AttackTimerHandle;

		FTimerHandle m_DeathTimerHandle;

		FTimerHandle m_AttachTimerHandle;

		bool m_PatrolIdle;

		bool m_WasKilled;

		float m_LastWaypointChange;

		bool m_IsStunned;

		TArray<AActor*> m_AttachedActors;

		TMap<AActor*, int32> m_AggroMap;

		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Behavior)
		int32 m_Health;

		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Behavior)
		int32 m_Level;

		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Behavior)
		EBotStates m_BotState;

		UPROPERTY(ReplicatedUsing = OnRep_BodyColorChanged, EditAnywhere, BlueprintReadWrite, Category = Character)
		FLinearColor m_BodyColor;

		UPROPERTY(ReplicatedUsing = OnRep_MaterialChanged, EditAnywhere, BlueprintReadWrite, Category = Character)
		UMaterialInterface* m_ChosenMaterial;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		TArray<UMaterialInterface*> m_RandomMaterials;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_ChaseSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_PatrolSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_FleeSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		ETargetTypes m_BotType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_WaypointLifetime;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_WaypointTolerance;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_TargetSearchInterval;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_AggroRange;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_UnAggroRange;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_ZTolerance;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_AttackRange;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		float m_AttackSpeed;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		int32 m_MaxHealth;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		FVector2D m_DamageRange;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		FVector2D m_PatrolRange;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		FVector2D m_PatrolIdleTimeRange;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		FString m_Faction;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
		TArray<FString> m_EnemyFactions;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		TArray<UAnimMontage*> m_AttackMontages;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* m_HurtMontage;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		UAnimMontage* m_DeathMontage;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		TSubclassOf<AActor> m_WornEquipClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		TSubclassOf<ALootableActorParent> m_LootActorClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		USoundCue* m_AggroSound;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		TArray<FString> m_LootTable;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		TArray<FQuestLoot> m_QuestLootTable;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		FVector2D m_LootAmountRange;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 m_HealthLevelMultiplier;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 m_BaseXP;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		FVector2D m_LevelRange;

		void StartAnimMontage(UAnimMontage* animMontage, float playRate = 1.0, FName startSectionName = NAME_None);

		void StopAllAnimMontages();

		void SearchForTarget();

		void BeginIdling();

		void FinishIdling();

		bool MoveToRandomLocation();

		void AttemptAttack();

		void InflictDamage(int32 damageAmount, AActor* theActor);

		void FinishDeath();

		void AttachEquipment(int32 itemID);

		void AttemptAttachment();

		bool IsTargetValid();

		bool IsTargetOutOfRange();

		bool IsTargetOutOfAttackRange();

		bool IsNearDeath();

		void SetBotState(EBotStates botState);

		void TriggerStun(float length);

		AActor* GetHighestAggro();

		UFUNCTION()
		void OnRep_BodyColorChanged();

		UFUNCTION()
		void OnRep_MaterialChanged();

		UFUNCTION(NetMulticast, Reliable)
		void ClientPlayAnimMontage(UAnimMontage* animMontage, float playRate = 1.0, FName startSectionName = NAME_None);
		void ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage, float playRate = 1.0, FName startSectionName = NAME_None);
		
		UFUNCTION(NetMulticast, Reliable)
		void ClientStopAllAnimMontages();
		void ClientStopAllAnimMontages_Implementation();

		UFUNCTION(NetMulticast, Reliable)
		void ClientPlaySoundAtLocation(USoundCue* soundCue, FVector location);
		void ClientPlaySoundAtLocation_Implementation(USoundCue* soundCue, FVector location);

		UFUNCTION()
		virtual void Destroyed() override;

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;
	
		// Called every frame
		virtual void Tick( float DeltaSeconds ) override;

		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
};