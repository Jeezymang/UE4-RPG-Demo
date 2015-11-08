// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "QuestLogSlotWidget.h"
#include "UMG/QuestLogPanelWidget.h"
#include "RPGDemoCharacter.h"

void UQuestLogSlotWidget::GenerateQuestInfo()
{
	UQuestLogPanelWidget* logPanel = Cast<UQuestLogPanelWidget>(m_ParentPanel);
	if (!logPanel || !logPanel->m_OwningPlayer) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(logPanel->m_OwningPlayer);
	if (!thePlayer) return;
	int32 questIndex = -1;
	for (int i = 0; i < thePlayer->m_Quests.Num(); i++)
	{
		if (thePlayer->m_Quests[i].Name == m_QuestNameText)
		{
			questIndex = i;
			break;
		}
	}
	if (questIndex != -1 && UStaticFunctions::m_QuestMap.Contains(m_QuestNameText))
	{
		logPanel->m_SelectedQuest = m_QuestNameText;
		FQuestDetails questDetails = thePlayer->m_Quests[questIndex];
		logPanel->m_QuestNameText = m_QuestNameText;
		logPanel->m_QuestDescriptionText = UStaticFunctions::SplitStringToLines(questDetails.Description, logPanel->m_LineCharacterAmount);
		logPanel->m_QuestRewardsText = FString::FromInt(UStaticFunctions::m_QuestMap[m_QuestNameText].XPReward) + "XP";
		for (FString itemReward : UStaticFunctions::m_QuestMap[m_QuestNameText].ItemRewards)
		{
			logPanel->m_QuestRewardsText += "\n" + itemReward;
		}
		switch (questDetails.Type)
		{
			case EQuestTypes::ItemRetrieval:
				logPanel->m_QuestTypeText = "Item Retrieval";
				logPanel->m_QuestRequiredObjectText = UStaticFunctions::m_QuestMap[m_QuestNameText].ItemRetrievalProgress.ItemName;
				logPanel->m_QuestProgressText = FString::FromInt(questDetails.ItemRetrievalProgress.CurrentItems) + "/" + FString::FromInt(UStaticFunctions::m_QuestMap[m_QuestNameText].ItemRetrievalProgress.RequiredItems);
			break;
			case EQuestTypes::Slaying:
				logPanel->m_QuestTypeText = "Slaying";
				logPanel->m_QuestRequiredObjectText = UStaticFunctions::m_QuestMap[m_QuestNameText].SlayingProgress.FactionName;
				logPanel->m_QuestProgressText = FString::FromInt(questDetails.SlayingProgress.CurrentKills) + "/" + FString::FromInt(UStaticFunctions::m_QuestMap[m_QuestNameText].SlayingProgress.RequiredKills);
			break;
		}
	}
}

FReply UQuestLogSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!m_ParentPanel) return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	GenerateQuestInfo();
	m_BorderColor = m_BorderSelectedColor;
	TArray<UWidget*> slotArray = m_ParentPanel->FindAllWidgets(UQuestLogSlotWidget::StaticClass());
	for (UWidget* slotWidget : slotArray)
	{
		UQuestLogSlotWidget* questSlotWidget = Cast<UQuestLogSlotWidget>(slotWidget);
		if (questSlotWidget && questSlotWidget != this)
			questSlotWidget->m_BorderColor = questSlotWidget->m_BorderDefaultColor;
		
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UQuestLogSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UQuestLogPanelWidget* logPanel = Cast<UQuestLogPanelWidget>(m_ParentPanel);
	if (logPanel && logPanel->m_SelectedQuest != m_QuestNameText)
		m_BorderColor = m_BorderHoverColor;

	Super::OnMouseEnter(InGeometry, InMouseEvent);
}

void UQuestLogSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UQuestLogPanelWidget* logPanel = Cast<UQuestLogPanelWidget>(m_ParentPanel);
	if (logPanel && logPanel->m_SelectedQuest == m_QuestNameText)
		m_BorderColor = m_BorderSelectedColor;
	else
		m_BorderColor = m_BorderDefaultColor;
	Super::OnMouseLeave(InMouseEvent);
}

