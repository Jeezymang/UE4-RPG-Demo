// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "CharacterBarWidget.h"
#include "RPGDemoCharacter.h"
#include "UMG/CharacterBarSlotWidget.h"

void UCharacterBarWidget::SetupActions() {
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);
	if (!thePlayer) return;
	WidgetTree->ForEachWidget([&](UWidget* Widget) {
		UCharacterBarSlotWidget* actionSlotWidget = Cast<UCharacterBarSlotWidget>(Widget);
		if (actionSlotWidget)
		{
			if (actionSlotWidget->SlotName == "Equipment")
				actionSlotWidget->FunctionDelegate.BindUObject(thePlayer, &ARPGDemoCharacter::ToggleEquipmentPanel);
			else if (actionSlotWidget->SlotName == "Inventory")
				actionSlotWidget->FunctionDelegate.BindUObject(thePlayer, &ARPGDemoCharacter::ToggleInventoryPanel);
			else if (actionSlotWidget->SlotName == "QuestLog")
				actionSlotWidget->FunctionDelegate.BindUObject(thePlayer, &ARPGDemoCharacter::ToggleQuestLogPanel);
		}
	});
}