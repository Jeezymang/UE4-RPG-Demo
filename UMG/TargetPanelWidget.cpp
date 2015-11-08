// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "TargetPanelWidget.h"
#include "RPGDemoCharacter.h"
#include "Bots/BotCharacterParent.h"

void UTargetPanelWidget::SetTargetActor(AActor* theTarget)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theTarget);
	ABotCharacterParent* theBot = Cast<ABotCharacterParent>(theTarget);
	if (!thePlayer && !theBot) return;
	m_TargetActor = theTarget;
	if (thePlayer)
	{
		m_TargetName = "Name: " + thePlayer->m_DisplayName;
		m_LastHealth = thePlayer->m_Health;
		m_LastEnergy = thePlayer->m_Energy;
		m_TargetHealthText = FString::FromInt(thePlayer->m_Health) + "/" + FString::FromInt(thePlayer->m_MaxHealth);
		m_TargetEnergyText = FString::FromInt(thePlayer->m_Energy) + "/" + FString::FromInt(thePlayer->m_MaxEnergy);
		m_TargetTexture = UStaticFunctions::LoadTextureFromPath("/Game/Textures/Icons/T_TargetPlayer");
		m_TargetLevel = "Lvl: " + FString::FromInt(thePlayer->m_Level);
	}
	else
	{
		if (!UStaticFunctions::m_TargetDataMap.Contains(theBot->m_BotType)) return;
		FTargetData targetData = (FTargetData)UStaticFunctions::m_TargetDataMap[theBot->m_BotType];
		m_TargetName = "Name: " + targetData.TargetName;
		m_LastHealth = theBot->m_Health;
		m_LastEnergy = 0;
		m_TargetHealthText = m_TargetHealthText = FString::FromInt(theBot->m_Health) + "/" + FString::FromInt(theBot->m_MaxHealth + (theBot->m_Level + theBot->m_HealthLevelMultiplier));
		m_TargetEnergyText = "0/0";
		m_TargetTexture = UStaticFunctions::LoadTextureFromPath(targetData.IconPath);
		m_TargetLevel = "Lvl: " + FString::FromInt(theBot->m_Level);
	}
}

void UTargetPanelWidget::ClearTargetActor()
{
	m_TargetActor = NULL;
	m_TargetName = "No Target";
	m_TargetHealthPercent = 1.0;
	m_TargetEnergyPercent = 1.0;
	m_TargetHealthText = "100/100";
	m_TargetEnergyText = "100/100";
	m_TargetTexture = UStaticFunctions::LoadTextureFromPath("/Game/Textures/Icons/UE4_LOGO_CARD");
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UTargetPanelWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!m_TargetActor) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_TargetActor);
	ABotCharacterParent* theBot = Cast<ABotCharacterParent>(m_TargetActor);

	int32 currentHealth = 0;
	int32 currentEnergy = 0;
	int32 maxHealth = 0;
	int32 maxEnergy = 0;
	if (thePlayer)
	{
		currentHealth = thePlayer->m_Health;
		currentEnergy = thePlayer->m_Energy;
		maxHealth = thePlayer->m_MaxHealth;
		maxEnergy = thePlayer->m_MaxEnergy;
	}
	else if (theBot)
	{
		currentHealth = theBot->m_Health;
		maxHealth = theBot->m_MaxHealth + (theBot->m_Level * theBot->m_HealthLevelMultiplier);
	}
	if (currentHealth == 0)
	{
		ClearTargetActor();
		return;
	}
		
	if (m_LastHealth == currentHealth)
		m_LastHealth = FMath::Min(maxHealth, currentHealth);
	else if (currentHealth == maxHealth)
		m_LastHealth = FMath::Min(maxHealth, FMath::Lerp(m_LastHealth, maxHealth, 0.1));
	else
		m_LastHealth = FMath::Min(maxHealth, FMath::Lerp(m_LastHealth, currentHealth, 0.1));

	if (m_LastEnergy == currentEnergy)
		m_LastEnergy = FMath::Min(maxEnergy, currentEnergy);
	else
		m_LastEnergy = FMath::Min(maxEnergy, FMath::Lerp(m_LastEnergy, currentEnergy, 0.1));
	
	m_TargetHealthText = FString::FromInt(m_LastHealth) + "/" + FString::FromInt(maxHealth);
	m_TargetEnergyText = FString::FromInt(m_LastEnergy) + "/" + FString::FromInt(maxEnergy);
	if (m_LastHealth == 0 || maxHealth == 0)
		m_TargetHealthPercent = 0;
	else
		m_TargetHealthPercent = ((float)m_LastHealth / (float)maxHealth);
	if (m_LastEnergy == 0 || maxEnergy == 0)
		m_TargetEnergyPercent = 0;
	else
		m_TargetEnergyPercent = ((float)m_LastEnergy / (float)maxEnergy);
}
