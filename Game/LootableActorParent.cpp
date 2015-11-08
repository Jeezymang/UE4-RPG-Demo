// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "LootableActorParent.h"
#include "UnrealNetwork.h"
#include "RPGDemoCharacter.h"
#include "UMG/LootPanelWidget.h"

// Sets default values
ALootableActorParent::ALootableActorParent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ALootableActorParent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALootableActorParent, m_Opened);
	DOREPLIFETIME(ALootableActorParent, m_HasLoot);
	DOREPLIFETIME(ALootableActorParent, m_Loot);
}

// Called when the game starts or when spawned
void ALootableActorParent::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Lootable");
	UStaticFunctions::SetObjectForDestruction(GetWorld(), this, 30.0);
}

// Called every frame
void ALootableActorParent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void ALootableActorParent::ToggleStatus(AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer) return;

	if (m_Opened)
	{
		m_CurrentLooter = NULL;
		m_Opened = false;
		GetWorld()->GetTimerManager().ClearTimer(m_AutoCloseHandle);
	}
	else if (!m_Opened && m_HasLoot)
	{
		for (TActorIterator<ALootableActorParent> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			if (ActorItr->m_CurrentLooter == theActor)
				ActorItr->m_CurrentLooter = NULL;
		}
		m_CurrentLooter = theActor;
		m_Opened = true;
		thePlayer->ClientPlayAnimMontage(thePlayer->m_AnimMontages.Loot);
		thePlayer->ClientOpenLootPanel(this);
		GetWorld()->GetTimerManager().SetTimer(m_AutoCloseHandle, this, &ALootableActorParent::AutoClose, 5, false);
	}
}

void ALootableActorParent::AutoClose()
{
	if (!m_Opened) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_CurrentLooter);
	if (thePlayer)
		thePlayer->ClientCloseLootPanel();

	m_CurrentLooter = NULL;
	m_Opened = false;
}

void ALootableActorParent::TriggerDestruction()
{
	this->Destroy();
}

void ALootableActorParent::OnRep_LootChanged()
{
	
	if (!m_CurrentLooter) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_CurrentLooter);
	if (!thePlayer) return;
	if (!thePlayer->m_Widgets.LootWidget) return;
	ULootPanelWidget* castedLootWidget = Cast<ULootPanelWidget>(thePlayer->m_Widgets.LootWidget);
	castedLootWidget->GenerateLootSlots();
}

