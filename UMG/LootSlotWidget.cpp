// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "LootSlotWidget.h"
#include "UMG/LootPanelWidget.h"
#include "RPGDemoCharacter.h"
#include "Game/LootableActorParent.h"

FReply ULootSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!m_ParentPanel) return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	ULootPanelWidget* lootPanel = Cast<ULootPanelWidget>(m_ParentPanel);
	if (!lootPanel || !lootPanel->m_OwningPlayer || !lootPanel->m_OwningObject) return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(lootPanel->m_OwningPlayer);
	if (!thePlayer) return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	thePlayer->ServerAttemptLootChest(lootPanel->m_OwningObject, m_ItemID);
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void ULootSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	m_BorderColor = m_BorderHoverColor;
	Super::OnMouseEnter(InGeometry, InMouseEvent);
}

void ULootSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	m_BorderColor = m_BorderDefaultColor;
	Super::OnMouseLeave(InMouseEvent);
}


