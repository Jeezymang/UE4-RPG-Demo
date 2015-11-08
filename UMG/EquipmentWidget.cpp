// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "EquipmentWidget.h"
#include "UMG/EquipmentSlotWidget.h"
#include "Game/WornEquipmentActorParent.h"
#include "RPGDemoCharacter.h"

void UEquipmentWidget::GenerateEquipmentIcons() {
	if (!m_OwningPlayer) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);
	if (!thePlayer) return;
	FEquipmentStats equipStats = UStaticFunctions::GetEquipmentStats(m_OwningPlayer);
	m_AttackPointsText = FString::FromInt(equipStats.AttackPoints);
	m_DefensePointsText = FString::FromInt(equipStats.DefensePoints);
	m_HealthPointsText = FString::FromInt(equipStats.HealthPoints);
	m_EnergyPointsText = FString::FromInt(equipStats.EnergyPoints);
	m_AttackSpeedText = FString::SanitizeFloat(equipStats.AttackSpeed);

	int32 widgetCount = 0;
	WidgetTree->ForEachWidget([&](UWidget* Widget) {
		UEquipmentSlotWidget* equipSlotWidget = Cast<UEquipmentSlotWidget>(Widget);
		if (equipSlotWidget){
			equipSlotWidget->m_ParentPanel = this;
			EEquipmentSlot equipSlot = equipSlotWidget->SlotType;
			if (!thePlayer->IsEquipmentSlotEmpty(equipSlot)) {
				AWornEquipmentActorParent* equipObject = UStaticFunctions::GetEquipmentSlot(m_OwningPlayer, equipSlot);
				if (!equipObject) return;
				int32 itemID = UStaticFunctions::GetItemIDByName(equipObject->m_ItemName);
				if (UStaticFunctions::ItemIDExists(itemID))
				{
					FItemDetails itemStruct = UStaticFunctions::m_ItemMap[itemID];
					equipSlotWidget->ItemID = itemID;
					equipSlotWidget->ItemName = itemStruct.ItemName;
					equipSlotWidget->ItemDescription = itemStruct.ItemDescription;
					equipSlotWidget->ItemTexture = UStaticFunctions::LoadTextureFromPath(itemStruct.ItemTexturePath);
					equipSlotWidget->FunctionDelegate.BindUObject(thePlayer, &ARPGDemoCharacter::ServerUnEquipSlot, equipSlot);
					equipSlotWidget->SlotColor = FLinearColor(1, 1, 1, 0.6);
				}
			}
			else {
				equipSlotWidget->ItemID = -1;
				equipSlotWidget->ItemTexture = UStaticFunctions::LoadTextureFromPath("/Game/Textures/Icons/T_InvSlotEmpty");
				equipSlotWidget->SlotColor = FLinearColor(1, 1, 1, 0.6);
			}
			equipSlotWidget->SlotNumber = widgetCount;
			widgetCount++;
		}
	});
}
