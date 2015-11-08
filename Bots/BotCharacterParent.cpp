// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "BotCharacterParent.h"
#include "Bots/BotAIController.h"
#include "RPGDemoCharacter.h"
#include "UnrealNetwork.h"
#include "Game/WornEquipmentActorParent.h"
#include "Game/LootableActorParent.h"

// Sets default values
ABotCharacterParent::ABotCharacterParent()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	AIControllerClass = ABotAIController::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
}

void ABotCharacterParent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABotCharacterParent, m_BotState);
	DOREPLIFETIME(ABotCharacterParent, m_Health);
	DOREPLIFETIME(ABotCharacterParent, m_Level);
	DOREPLIFETIME(ABotCharacterParent, m_BodyColor);
	DOREPLIFETIME(ABotCharacterParent, m_ChosenMaterial);
}

// Called when the game starts or when spawned
void ABotCharacterParent::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Targetable");

	ABotAIController* theController = Cast<ABotAIController>(Controller);
	if (theController) {
		m_Controller = theController;
		GetCharacterMovement()->MaxWalkSpeed = m_PatrolSpeed;
		m_BotState = EBotStates::Patrol;
		m_ChosenMaterial = m_RandomMaterials[FMath::RandRange(0, m_RandomMaterials.Num() - 1)];
		for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
		{
			GetMesh()->SetMaterial(i, m_ChosenMaterial);
			if (i == 0 && m_BotType == ETargetTypes::Character)
				break;
		}
		if (m_BotType == ETargetTypes::Character)
			m_BodyMaterialInstance = UMaterialInstanceDynamic::Create(m_ChosenMaterial, this);
		m_Level = FMath::RandRange((int)m_LevelRange.X, (int)m_LevelRange.Y);
		m_Health = m_MaxHealth + (m_HealthLevelMultiplier * m_Level);
	}

	if (!theController) return;

	m_NavSystem = UNavigationSystem::GetCurrent(GetWorld());
	if (!GetWorld() || !m_NavSystem || !m_Controller) return;
	GetWorld()->GetTimerManager().SetTimer(m_TargetSearchHandle, this, &ABotCharacterParent::SearchForTarget, m_TargetSearchInterval, true);
	GetWorld()->GetTimerManager().SetTimer(m_AttachTimerHandle, this, &ABotCharacterParent::AttemptAttachment, 1, false);
	if (m_BotType != ETargetTypes::Character) return;
	m_BodyColor = FLinearColor(FMath::FRandRange(0.3, 0.8), FMath::FRandRange(0.3, 0.8), FMath::FRandRange(0.3, 0.8), 1);
}

void ABotCharacterParent::Destroyed()
{
	Super::Destroyed();
	if (m_BotType == ETargetTypes::Murloc) return;
	if (m_AttachedActors.Num() == 0) return;
	for (int i = 0; i < m_AttachedActors.Num(); i++)
	{
		m_AttachedActors[i]->Destroy();
	}
}

// Called every frame
void ABotCharacterParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UWorld* theWorld = GetWorld();
	float currentTime = theWorld->GetRealTimeSeconds();
	if (m_WasKilled) return;
	if (!theWorld || !m_NavSystem || !m_Controller) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_Target);
	if (m_IsStunned) return;
	if (m_BotState == EBotStates::Attacking && m_Target)
	{
		FRotator lookAtRotation = FRotationMatrix::MakeFromX(m_Target->GetActorLocation() - GetActorLocation()).Rotator();
		FRotator actorRotation = GetActorRotation();
		SetActorRotation(FRotator(actorRotation.Pitch, lookAtRotation.Yaw, actorRotation.Roll));
	}

	switch (m_BotState)
	{
	case EBotStates::Patrol:
	{
		if (m_PatrolIdle) return;
		if (m_LastWaypointChange == 0)
		{
			if (MoveToRandomLocation())
				m_LastWaypointChange = GetWorld()->GetRealTimeSeconds();
		}
		else if (m_LastWaypointChange + m_WaypointLifetime < currentTime)
			BeginIdling();
		else if (FVector::Dist(GetActorLocation(), m_CurrentWaypoint) < m_WaypointTolerance)
			BeginIdling();
	}
	break;
	case EBotStates::Chasing:
	{
		if (!IsTargetValid() || IsTargetOutOfRange())
		{
			SetBotState(EBotStates::Patrol);
			return;
		}
		if (!IsTargetOutOfAttackRange())
		{
			SetBotState(EBotStates::Attacking);
			return;
		}
		if (IsNearDeath())
		{
			SetBotState(EBotStates::Fleeing);
			return;
		}
		m_Controller->MoveToActor(m_Target, 50.f);
	}
	break;
	case EBotStates::Attacking:
	{
		if (!IsTargetValid())
		{
			SetBotState(EBotStates::Patrol);
			return;
		}
		if (IsTargetOutOfAttackRange())
		{
			SetBotState(EBotStates::Chasing);
			return;
		}
		if (IsNearDeath())
		{
			SetBotState(EBotStates::Fleeing);
			return;
		}
		if (!theWorld->GetTimerManager().TimerExists(m_AttackTimerHandle))
			theWorld->GetTimerManager().SetTimer(m_AttackTimerHandle, this, &ABotCharacterParent::AttemptAttack, m_AttackSpeed, true);
	}
	break;
	case EBotStates::Fleeing:
	{
		if (!IsTargetValid())
		{
			SetBotState(EBotStates::Patrol);
			return;
		}
		if (IsTargetOutOfRange())
		{
			SetBotState(EBotStates::Patrol);
			return;
		}
		FVector targetLocation = m_Target->GetActorLocation();
		const FRotator rot(0, 180, 0);
		targetLocation = rot.RotateVector(targetLocation);
		m_Controller->MoveToLocation(targetLocation);
	}
	break;
	}
}

bool ABotCharacterParent::IsTargetValid()
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_Target);
	ABotCharacterParent* theBot = Cast<ABotCharacterParent>(m_Target);
	if (!thePlayer && !theBot) return false;
	if ((thePlayer && thePlayer->m_IsDead) || (theBot && theBot->m_WasKilled)) return false;
	return true;
}

bool ABotCharacterParent::IsTargetOutOfRange()
{
	if (!m_Target) return true;
	if (FVector::Dist(m_Target->GetActorLocation(), GetActorLocation()) > m_UnAggroRange || FMath::Abs(GetActorLocation().Z - m_Target->GetActorLocation().Z) > m_ZTolerance)
		return true;
	return false;
}

bool ABotCharacterParent::IsTargetOutOfAttackRange()
{
	if (!m_Target) return true;
	if (FVector::Dist(m_Target->GetActorLocation(), GetActorLocation()) > m_AttackRange || FMath::Abs(GetActorLocation().Z - m_Target->GetActorLocation().Z) > (m_ZTolerance * 0.25))
		return true;
	return false;
}

bool ABotCharacterParent::IsNearDeath()
{
	return (m_Health <= 0.25);
}

void ABotCharacterParent::SetBotState(EBotStates botState)
{
	switch (botState)
	{
	case EBotStates::Patrol:
		m_BotState = EBotStates::Patrol;
		GetCharacterMovement()->MaxWalkSpeed = m_PatrolSpeed;
		m_Target = NULL;
		GetWorld()->GetTimerManager().ClearTimer(m_AttackTimerHandle);
		break;
	case EBotStates::Chasing:
		m_BotState = EBotStates::Chasing;
		GetCharacterMovement()->MaxWalkSpeed = m_ChaseSpeed;
		m_Controller->MoveToActor(m_Target, 50.f);
		GetWorld()->GetTimerManager().ClearTimer(m_AttackTimerHandle);
		break;
	case EBotStates::Attacking:
		m_Controller->StopMovement();
		m_BotState = EBotStates::Attacking;
		break;
	case EBotStates::Fleeing:
		m_BotState = EBotStates::Fleeing;
		GetCharacterMovement()->MaxWalkSpeed = m_FleeSpeed;
		GetWorld()->GetTimerManager().ClearTimer(m_AttackTimerHandle);
		break;
	}
}

void ABotCharacterParent::SearchForTarget()
{
	if (m_BotState != EBotStates::Patrol) return;
	if (m_Target || m_WasKilled || m_IsStunned) return;
	for (TActorIterator<ARPGDemoCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		bool isInRange = (FVector::Dist(ActorItr->GetActorLocation(), GetActorLocation()) < m_AggroRange);
		bool isPlayer = (ActorItr->m_DisplayName != "");
		bool isInZTolerance = (FMath::Abs(GetActorLocation().Z - ActorItr->GetActorLocation().Z) < m_ZTolerance);
		if (isInRange && isPlayer && isInZTolerance && !ActorItr->m_IsDead && ActorItr->ActorHasTag("Targetable"))
		{
			AActor* theActor = Cast<AActor>(ActorItr->Controller->GetPawn());
			m_Target = theActor;
			SetBotState(EBotStates::Chasing);
			ClientPlaySoundAtLocation(m_AggroSound, GetActorLocation());
			break;
		}
	}
	if (m_Target) return;
	for (TActorIterator<ABotCharacterParent> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		bool isInRange = (FVector::Dist(ActorItr->GetActorLocation(), GetActorLocation()) < m_AggroRange);
		bool isDifferentFaction = (ActorItr->m_Faction != m_Faction);
		bool isInZTolerance = (FMath::Abs(GetActorLocation().Z - ActorItr->GetActorLocation().Z) < m_ZTolerance);
		if (isInRange && isDifferentFaction && isInZTolerance && m_Health > 0 && ActorItr->ActorHasTag("Targetable"))
		{
			if (m_EnemyFactions.Contains(ActorItr->m_Faction))
			{
				AActor* theActor = Cast<AActor>(ActorItr->Controller->GetPawn());
				m_Target = theActor;
				SetBotState(EBotStates::Chasing);
				ClientPlaySoundAtLocation(m_AggroSound, GetActorLocation());
				break;
			}
		}
	}
}

void ABotCharacterParent::AttemptAttack()
{
	if (!m_Target || m_IsStunned) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_Target);
	ABotCharacterParent* theBot = Cast<ABotCharacterParent>(m_Target);
	if ((!thePlayer && !theBot) || ((thePlayer && thePlayer->m_IsDead) || (theBot && theBot->m_WasKilled))) return;
	UAnimMontage* attackMontage = m_AttackMontages[FMath::RandRange(0, m_AttackMontages.Num() - 1)];
	StartAnimMontage(attackMontage);
	ClientPlayAnimMontage(attackMontage);
	if (UStaticFunctions::ActorIsFacingLocation(this, m_Target->GetActorLocation()))
	{
		int32 damageAmount = FMath::RandRange(m_DamageRange.X, m_DamageRange.Y);
		if (thePlayer)
			thePlayer->InflictDamage(damageAmount, this);
		else if (theBot)
			theBot->InflictDamage(damageAmount, this);
	}
}

void ABotCharacterParent::BeginIdling()
{
	m_PatrolIdle = true;
	float randomIdleTime = FMath::FRandRange(m_PatrolIdleTimeRange.X, m_PatrolIdleTimeRange.Y);
	GetWorld()->GetTimerManager().SetTimer(m_PatrolIdleHandle, this, &ABotCharacterParent::FinishIdling, randomIdleTime, false);
}

void ABotCharacterParent::FinishIdling()
{
	if (m_WasKilled || m_IsStunned) return;
	if (m_BotState == EBotStates::Patrol)
	if (MoveToRandomLocation())
		m_LastWaypointChange = GetWorld()->GetRealTimeSeconds();

	m_PatrolIdle = false;
}

bool ABotCharacterParent::MoveToRandomLocation()
{
	if (m_IsStunned) return false;
	float radius = FMath::FRandRange(m_PatrolRange.X, m_PatrolRange.Y);
	FVector resultLocation;
	if (m_Controller->GetRandomPointInRadius(m_NavSystem, GetActorLocation(), radius, resultLocation))
	{
		m_CurrentWaypoint = resultLocation;
		m_Controller->MoveToLocation(m_CurrentWaypoint);
		return true;
	}
	return false;
}

void ABotCharacterParent::StartAnimMontage(UAnimMontage* animMontage, float playRate, FName startSectionName)
{
	USkeletalMeshComponent* useMesh = GetMesh();
	if (animMontage && useMesh && useMesh->AnimScriptInstance)
	{
		if (useMesh->AnimScriptInstance->Montage_IsPlaying(animMontage))
			return;
		useMesh->AnimScriptInstance->Montage_Play(animMontage, playRate);
	}
}

void ABotCharacterParent::StopAllAnimMontages()
{
	USkeletalMeshComponent* useMesh = GetMesh();
	if (useMesh && useMesh->AnimScriptInstance)
		useMesh->AnimScriptInstance->Montage_Stop(0.0f);
}

void ABotCharacterParent::InflictDamage(int32 damageAmount, AActor* theActor)
{
	if (m_WasKilled) return;
	if (!m_AggroMap.Contains(theActor))
		m_AggroMap.Add(theActor, 1);
	else
		m_AggroMap[theActor] = (int32)m_AggroMap[theActor] + 1;

	m_Target = GetHighestAggro();
	if (m_Target && m_BotState != EBotStates::Attacking)
		SetBotState(EBotStates::Chasing);
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	int32 randChance = FMath::RandRange(1, 3);
	bool attackMontageActive = false;
	for (UAnimMontage* animMontage : m_AttackMontages)
	{
		if (GetMesh()->AnimScriptInstance->Montage_IsPlaying(animMontage))
		{
			attackMontageActive = true;
			break;
		}
	}
	if (!attackMontageActive || (attackMontageActive && randChance > 2))
	{
		StartAnimMontage(m_HurtMontage);
		ClientPlayAnimMontage(m_HurtMontage);
	}
	m_Health = FMath::Clamp(m_Health - damageAmount, 0, m_MaxHealth);
	if (thePlayer)
	{
		FVector2D posOffset = FVector2D(FMath::FRandRange(0.3, 0.6), FMath::FRandRange(0.3, 0.6));
		FVector2D theOffset = FVector2D(FMath::FRandRange(-50, 50), FMath::FRandRange(-100, -50));
		thePlayer->ClientAddFloatingTextEx("-" + FString::FromInt(damageAmount), FLinearColor(0.5, 0, 0, 1), "/Game/Fonts/Homoarakhn", FVector2D(2, 2), posOffset, theOffset, 2.5f, false, false, 0, true, FLinearColor(1, 0, 0, 1), true, 3);
	}

	if (m_BotState == EBotStates::Fleeing && m_Target)
		m_Target = theActor;
	if (m_Health == 0)
	{
		int32 xpReward = m_BaseXP + ((m_Level + (m_Level / 2)))*((m_Level / 2) + (m_Level / 5));
		if (thePlayer)
		{
			thePlayer->UpdateQuestProgress(this, "", 1);
			thePlayer->GiveXP(xpReward);
		}
		m_WasKilled = true;
		UWorld* theWorld = GetWorld();
		theWorld->GetTimerManager().ClearTimer(m_TargetSearchHandle);
		theWorld->GetTimerManager().ClearTimer(m_AttackTimerHandle);
		StopAllAnimMontages();
		ClientStopAllAnimMontages();
		StartAnimMontage(m_DeathMontage);
		ClientPlayAnimMontage(m_DeathMontage);
		m_Killer = theActor;
		for (TActorIterator<ABotCharacterParent> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->m_AggroMap.Contains(this))
				ActorItr->m_AggroMap.Remove(this);
			if (ActorItr->m_Target == this)
				ActorItr->m_Target = NULL;
		}
		int32 deathTime = 3;
		if (m_BotType == ETargetTypes::Dragonoid)
			deathTime = 1;
		GetWorld()->GetTimerManager().SetTimer(m_DeathTimerHandle, this, &ABotCharacterParent::FinishDeath, deathTime, false);
		Tags.Remove("Targetable");
	}
}

AActor* ABotCharacterParent::GetHighestAggro()
{
	AActor* aggroActor = NULL;
	int32 highestAggro = 0;
	TArray<AActor*> actorsToRemove;
	for (auto& element : m_AggroMap)
	{
		int32 aggroValue = element.Value;
		AActor* aActor = element.Key;
		if (!aActor || FVector::Dist(aActor->GetActorLocation(), GetActorLocation()) > m_AggroRange)
			actorsToRemove.Add(aActor);
		else if (aggroValue > highestAggro)
		{
			aggroActor = element.Key;
			highestAggro = aggroValue;
		}
	}
	for (AActor* aActor : actorsToRemove)
	{
		if (m_AggroMap.Contains(aActor))
			m_AggroMap.Remove(aActor);
	}
	return aggroActor;
}

void ABotCharacterParent::FinishDeath()
{
	FVector chestOffset = FVector(0, 0, UStaticFunctions::GetActorCollisionBounds(this).Z);
	AActor* theActor = GetWorld()->SpawnActor(m_LootActorClass);
	if (!theActor) return;

	theActor->SetActorLocation(GetActorLocation() - chestOffset);
	ALootableActorParent* theLoot = Cast<ALootableActorParent>(theActor);
	if (!theLoot) return;

	int randAmt = FMath::RandRange((int)m_LootAmountRange.X, (int)m_LootAmountRange.Y);
	for (int i = 0; i < randAmt; i++)
	{
		FString randItem = m_LootTable[FMath::RandRange(0, m_LootTable.Num() - 1)];
		int32 itemID = UStaticFunctions::GetItemIDByName(randItem);
		if (UStaticFunctions::ItemIDExists(itemID))
			theLoot->m_Loot.Add(itemID);
	}
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_Killer);
	if (thePlayer)
	{
		for (FQuestLoot questLoot : m_QuestLootTable)
		{
			int randChance = FMath::RandRange(0, 100);
			if (FMath::Clamp((int)(questLoot.DropChance * 100), 0, 100) <= randChance)
			{
				if (thePlayer->HasQuest(questLoot.QuestName) && !thePlayer->IsQuestComplete(questLoot.QuestName))
				{
					int32 itemID = UStaticFunctions::GetItemIDByName(questLoot.ItemName);
					if (UStaticFunctions::ItemIDExists(itemID))
						theLoot->m_Loot.Add(itemID);
				}
			}
		}
	}
	this->Destroy();
}

void ABotCharacterParent::AttachEquipment(int32 itemID)
{
	if (!UStaticFunctions::m_EquipmentClassMap.Contains(itemID)) return;
	AActor* spawnedActor = GetWorld()->SpawnActor(m_WornEquipClass);
	AWornEquipmentActorParent* theEquipment = Cast<AWornEquipmentActorParent>(spawnedActor);
	if (theEquipment)
	{
		FWornEquipmentData equipData = UStaticFunctions::m_WornEquipmentDataMap[itemID];
		theEquipment->m_SocketName = equipData.SocketName;
		theEquipment->m_Offset = equipData.Offset;
		theEquipment->m_Rotation = equipData.Rotation;
		theEquipment->m_Scale = equipData.Scale;
		theEquipment->m_ItemName = UStaticFunctions::m_ItemMap[itemID].ItemName;
		FVector socketLocation = GetMesh()->GetSocketLocation(equipData.SocketName);
		theEquipment->m_StaticMesh->SetStaticMesh(UStaticFunctions::LoadStaticMeshFromPath(UStaticFunctions::m_EquipmentClassMap[itemID]));
		spawnedActor->SetActorRelativeRotation(GetMesh()->GetSocketRotation(equipData.SocketName));
		spawnedActor->AddActorLocalRotation(FQuat::MakeFromEuler(equipData.Rotation));
		spawnedActor->SetActorRelativeLocation(socketLocation);
		spawnedActor->AddActorLocalOffset(equipData.Offset);
		spawnedActor->GetRootComponent()->SetWorldScale3D(equipData.Scale);
		spawnedActor->GetRootComponent()->AttachTo(GetMesh(), equipData.SocketName, EAttachLocation::KeepWorldPosition);
		m_AttachedActors.Add(spawnedActor);
	}
}

void ABotCharacterParent::AttemptAttachment()
{
	if (m_BotType != ETargetTypes::Character) return;
	int itemIndex = FMath::RandRange(0, 6);
	int randChance = FMath::RandRange(0, 5);
	if (randChance < 3)
		AttachEquipment(itemIndex);
}

void ABotCharacterParent::TriggerStun(float length)
{
	if (m_IsStunned) return;
	m_IsStunned = true;
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]{
		if (this)
			m_IsStunned = false;
	});
	UStaticFunctions::TriggerDelayedAction(GetWorld(), timerDelegate, length, false);
	Controller->StopMovement();
}

void ABotCharacterParent::ClientPlaySoundAtLocation_Implementation(USoundCue* soundCue, FVector location)
{
	UGameplayStatics::PlaySoundAtLocation(this, soundCue, location);
}

void ABotCharacterParent::ClientStopAllAnimMontages_Implementation()
{
	StopAllAnimMontages();
}

void ABotCharacterParent::ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage, float playRate, FName startSectionName)
{
	StartAnimMontage(animMontage, playRate, startSectionName);
}

// Called to bind functionality to input
void ABotCharacterParent::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ABotCharacterParent::OnRep_BodyColorChanged()
{
	if (m_BotType != ETargetTypes::Character) return;
	if (!m_BodyMaterialInstance)
	{
		m_BodyMaterialInstance = UMaterialInstanceDynamic::Create(m_ChosenMaterial, this);
		m_ChosenMaterial = m_BodyMaterialInstance;
		//GetMesh()->SetMaterial(0, m_BodyMaterialInstance);
	}
	m_BodyMaterialInstance->SetVectorParameterValue(TEXT("BodyColor"), m_BodyColor);
}

void ABotCharacterParent::OnRep_MaterialChanged()
{
	for (int i = 0; i < GetMesh()->GetMaterials().Num(); i++)
	{
		GetMesh()->SetMaterial(i, m_ChosenMaterial);
		if (i == 0 && m_BotType == ETargetTypes::Character)
			break;
	}
}