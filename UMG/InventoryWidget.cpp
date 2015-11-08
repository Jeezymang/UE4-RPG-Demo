// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "InventoryWidget.h"
#include "UMG/InventorySlotWidget.h"
#include "RPGDemoCharacter.h"

void UInventoryWidget::GenerateInventoryIcons() {
	int32 index = 0;
	if (!m_OwningPlayer) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);
	if (!thePlayer) return;
	WidgetTree->ForEachWidget([&](UWidget* Widget) {
		UInventorySlotWidget* inventorySlotWidget = Cast<UInventorySlotWidget>(Widget);
		if (inventorySlotWidget){
			inventorySlotWidget->m_ParentPanel = this;
			if (thePlayer->m_Inventory.IsValidIndex(index)) {
				int32 itemID = thePlayer->m_Inventory[index];
				if (UStaticFunctions::ItemIDExists(itemID))
				{
					FItemDetails itemStruct = UStaticFunctions::m_ItemMap[itemID];
					inventorySlotWidget->ItemSlot = index;
					inventorySlotWidget->ItemID = itemID;
					inventorySlotWidget->ItemName = itemStruct.ItemName;
					inventorySlotWidget->ItemTexture = UStaticFunctions::LoadTextureFromPath(itemStruct.ItemTexturePath);
				}
			}
			else {
				inventorySlotWidget->ItemSlot = -1;
				inventorySlotWidget->ItemID = -1;
				inventorySlotWidget->ItemName = "Empty";
				inventorySlotWidget->ItemTexture = UStaticFunctions::LoadTextureFromPath("/Game/Textures/Icons/T_InvSlotEmpty");
			}
			index++;
		}
	});
}
