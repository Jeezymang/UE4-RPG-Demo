// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "LootPanelWidget.h"
#include "UMG/LootSlotWidget.h"
#include "Game/LootableActorParent.h"
#include "RPGDemoCharacter.h"

void ULootPanelWidget::GenerateLootSlots()
{
	if (!m_OwningPlayer) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);

	if (!thePlayer) return;

	if (!m_OwningObject) return;
	ALootableActorParent* theObject = Cast<ALootableActorParent>(m_OwningObject);

	if (!theObject) return;
	
	UScrollBox* scrollPanel = (UScrollBox*)FindFirstWidget(UScrollBox::StaticClass());
	if (!scrollPanel) return;
	scrollPanel->ClearChildren();

	if (theObject->m_Loot.Num() == 0) return;

	for (int i = 0; i < theObject->m_Loot.Num(); i++)
	{
		if (!UStaticFunctions::ItemIDExists(theObject->m_Loot[i])) continue;
		FItemDetails itemStruct = UStaticFunctions::m_ItemMap[theObject->m_Loot[i]];
		UCanvasPanel* canvasPanel = NewObject<UCanvasPanel>(USizeBox::StaticClass());
		canvasPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		UCustomUserWidgetParent* slotWidget = CreateWidget<UCustomUserWidgetParent>(thePlayer->Controller->CastToPlayerController(), thePlayer->m_WidgetClasses.LootSlotWidget);
		ULootSlotWidget* lootSlot = Cast<ULootSlotWidget>(slotWidget);
		lootSlot->m_ParentPanel = this;
		lootSlot->m_ItemID = theObject->m_Loot[i];
		lootSlot->m_ItemName = itemStruct.ItemName;
		lootSlot->m_ItemDescription = UStaticFunctions::SplitStringToLines(itemStruct.ItemDescription, m_LineCharacterAmount);
		lootSlot->m_ItemTexture = UStaticFunctions::LoadTextureFromPath(itemStruct.ItemTexturePath);
		UPanelSlot* panelSlot = canvasPanel->AddChild(slotWidget);
		UCanvasPanelSlot* canvasPanelSlot = Cast<UCanvasPanelSlot>(panelSlot);
		FAnchors theAnchors = FAnchors();
		theAnchors.Maximum = FVector2D(0, 0);
		theAnchors.Minimum = FVector2D(0, 0);
		canvasPanelSlot->SetSize(m_LootSlotSize);
		canvasPanelSlot->SetAnchors(theAnchors);
		UScrollBoxSlot* theSlot = Cast<UScrollBoxSlot>(scrollPanel->AddChild(canvasPanel));
		theSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
	}
}

