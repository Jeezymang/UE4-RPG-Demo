// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "PlayerVitalsWidget.h"
#include "RPGDemoCharacter.h"

void UPlayerVitalsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!m_OwningPlayer) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);

	if (!thePlayer) return;

	int32 currentHealth = 0;
	int32 currentEnergy = 0;
	int32 maxHealth = 0;
	int32 maxEnergy = 0;

	currentHealth = thePlayer->m_Health;
	currentEnergy = thePlayer->m_Energy;
	maxHealth = thePlayer->m_MaxHealth;
	maxEnergy = thePlayer->m_MaxEnergy;

	if (m_LastHealth == currentHealth)
		m_LastHealth = FMath::Min(maxHealth, currentHealth);
	else
		m_LastHealth = FMath::Min(maxHealth, FMath::Lerp(m_LastHealth, currentHealth, 0.1));

	if (m_LastEnergy == currentEnergy)
		m_LastEnergy = FMath::Min(maxEnergy, currentEnergy);
	else
		m_LastEnergy = FMath::Min(maxEnergy, FMath::Lerp(m_LastEnergy, currentEnergy, 0.1));

	m_HealthText = FString::FromInt(m_LastHealth) + "/" + FString::FromInt(maxHealth);
	m_EnergyText = FString::FromInt(m_LastEnergy) + "/" + FString::FromInt(maxEnergy);
	if (m_LastHealth == 0 || maxHealth == 0)
		m_HealthPercent = 0;
	else
		m_HealthPercent = ((float)m_LastHealth / (float)maxHealth);
	if (m_LastEnergy == 0 || maxEnergy == 0)
		m_EnergyPercent = 0;
	else
		m_EnergyPercent = ((float)m_LastEnergy / (float)maxEnergy);

	m_Level = FString::FromInt(thePlayer->m_Level);
	int32 lastLevelXP = thePlayer->RequiredXPToNextLevel(thePlayer->m_Level);
	int32 nextLevelXP = thePlayer->RequiredXPToNextLevel(thePlayer->m_Level + 1);
	m_ExperienceText = FString::FromInt(thePlayer->m_Experience) + "/" + FString::FromInt(nextLevelXP);
	m_LevelPercent = (float)(((float)(thePlayer->m_Experience - lastLevelXP)) / ((float)(nextLevelXP - lastLevelXP)));
}