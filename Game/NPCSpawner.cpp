// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "NPCSpawner.h"
#include "Bots/BotCharacterParent.h"


// Sets default values
ANPCSpawner::ANPCSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPCSpawner::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority)
	{
		GetWorld()->GetTimerManager().SetTimer(m_SpawnDelegate, this, &ANPCSpawner::SpawnTrigger, m_SpawnInterval, true);
	}
}

// Called every frame
void ANPCSpawner::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ANPCSpawner::SpawnTrigger()
{
	int actorCount = 0;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ActorItr->IsA(ABotCharacterParent::StaticClass()))
			actorCount++;
	}
	if (actorCount >= m_NPCLimit)
		return;
	float randChance = FMath::FRandRange(0, 100);
	float chanceNum = FMath::Clamp(m_SpawnChance * 100.0, 0.0, 100.0);
	if (randChance > chanceNum) return;
	AActor* theActor = GetWorld()->SpawnActor(m_NPCClass);
	ABotCharacterParent* theBot = Cast<ABotCharacterParent>(theActor);
	theBot->SpawnDefaultController();
	theBot->m_NavSystem = GetWorld()->GetNavigationSystem();
	theActor->SetActorLocation(GetActorLocation());
}