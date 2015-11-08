// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "QuestLogPanelWidget.h"
#include "RPGDemoCharacter.h"
#include "UMG/QuestLogSlotWidget.h"

void UQuestLogPanelWidget::GenerateQuestSlots()
{
	if (!m_OwningPlayer) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);

	if (!thePlayer) return;

	UScrollBox* scrollPanel = (UScrollBox*)FindFirstWidget(UScrollBox::StaticClass());
	if (!scrollPanel) return;
	scrollPanel->ClearChildren();

	if (thePlayer->m_Quests.Num() == 0) return;

	for (int i = 0; i < thePlayer->m_Quests.Num(); i++)
	{
		UCanvasPanel* canvasPanel = NewObject<UCanvasPanel>(USizeBox::StaticClass());
		canvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UCustomUserWidgetParent* slotWidget = CreateWidget<UCustomUserWidgetParent>(thePlayer->Controller->CastToPlayerController(), thePlayer->m_WidgetClasses.QuestLogSlotWidget);
		UQuestLogSlotWidget* questSlot = Cast<UQuestLogSlotWidget>(slotWidget);
		questSlot->m_ParentPanel = this;
		questSlot->m_QuestNameText = thePlayer->m_Quests[i].Name;
		if (thePlayer->m_Quests[i].Completed)
			questSlot->m_ImageTexture = UStaticFunctions::LoadTextureFromPath("/Game/Textures/UI/T_CheckMark");
		UPanelSlot* panelSlot = canvasPanel->AddChild(slotWidget);
		UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(panelSlot);
		FAnchors theAnchors = FAnchors();
		theAnchors.Maximum = FVector2D(0, 0);
		theAnchors.Minimum = FVector2D(0, 0);
		canvasPanelSlot->SetSize(m_QuestSlotSize);
		canvasPanelSlot->SetAnchors(theAnchors);
		UScrollBoxSlot* theSlot = Cast<UScrollBoxSlot>(scrollPanel->AddChild(canvasPanel));
		theSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
		if (questSlot->m_QuestNameText == m_SelectedQuest) {
			questSlot->GenerateQuestInfo();
		}
	}
}



