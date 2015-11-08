// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "InventoryOptionsWidget.h"
#include "UMG/InventoryWidget.h"
#include "UMG/CustomButtonWidget.h"
#include "RPGDemoCharacter.h"

void UInventoryOptionsWidget::ConsumeItem()
{
	if (!m_ParentPanel) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_ParentPanel->m_OwningPlayer);
	if (!thePlayer) return;
	FItemDetails inventoryStruct = UStaticFunctions::m_ItemMap[ItemID];
	if (inventoryStruct.ItemType == EItemType::Armor || inventoryStruct.ItemType == EItemType::Weapon)
		thePlayer->ServerEquipSlotByID(ItemID);
	else if (inventoryStruct.ItemType == EItemType::Consumable)
		thePlayer->ServerConsumeItem(ItemID);
	thePlayer->m_Widgets.InventoryOptionsWidget = NULL;
	this->RemoveFromViewport();
}

void UInventoryOptionsWidget::RemoveItem()
{
	if (!m_ParentPanel) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_ParentPanel->m_OwningPlayer);
	if (!thePlayer) return;
	thePlayer->ServerRemoveInventoryItem(ItemID);
	thePlayer->m_Widgets.InventoryOptionsWidget = NULL;
	this->RemoveFromViewport();
}

void UInventoryOptionsWidget::Cancel()
{
	if (!m_ParentPanel) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_ParentPanel->m_OwningPlayer);
	if (!thePlayer) return;
	thePlayer->m_Widgets.InventoryOptionsWidget = NULL;
	this->RemoveFromViewport();
}

void UInventoryOptionsWidget::SetActionButtonText()
{
	FItemDetails itemStruct = UStaticFunctions::m_ItemMap[ItemID];
	switch (itemStruct.ItemType)
	{
		case EItemType::Consumable:
			ActionButtonText = "Consume";
		break;
		case EItemType::Armor:
			ActionButtonText = "Equip";
		break;
		case EItemType::Weapon:
			ActionButtonText = "Equip";
		break;
		case EItemType::QuestItem:
			ActionButtonText = "NULL";
		break;
		/*case EItemType::Use:
			ActionButtonText = "Use";
		break;*/
	}
}

void UInventoryOptionsWidget::SetupButtonFunctions()
{
	SetActionButtonText();
	int32 index = 0;
	FItemDetails itemStruct = UStaticFunctions::m_ItemMap[ItemID];
	WidgetTree->ForEachWidget([&](UWidget* Widget) {
		UCustomButtonWidget* buttonWidget = Cast<UCustomButtonWidget>(Widget);
		if (buttonWidget){
			buttonWidget->DefaultButtonColor = buttonWidget->ButtonColor;
			switch (index)
			{
				case 0: // Consume
					buttonWidget->FunctionDelegate.BindUObject(this, &UInventoryOptionsWidget::ConsumeItem);
				break;
				case 1: // Remove
					buttonWidget->FunctionDelegate.BindUObject(this, &UInventoryOptionsWidget::RemoveItem);
				break;
				case 2: // Cancel
					buttonWidget->FunctionDelegate.BindUObject(this, &UInventoryOptionsWidget::Cancel);
				break;
				default:
					buttonWidget->FunctionDelegate.BindUObject(this, &UInventoryOptionsWidget::Cancel);
			}
			index++;
		}
	});
	if (itemStruct.ItemType == EItemType::QuestItem)
		RemoveFirstButton();
}

void UInventoryOptionsWidget::RemoveFirstButton()
{
	TArray<UWidget*> buttonWidgetArray = FindAllWidgets(UCustomButtonWidget::StaticClass());
	buttonWidgetArray[0]->RemoveFromParent();
	buttonWidgetArray.RemoveAt(0);
	for (UWidget* aWidget : buttonWidgetArray)
	{
		UCustomButtonWidget* buttonWidget = Cast<UCustomButtonWidget>(aWidget);
		UPanelSlot* panelSlot = buttonWidget->Slot;
		UUniformGridSlot* gridSlot = Cast<UUniformGridSlot>(panelSlot);
		gridSlot->SetRow(FMath::Clamp(gridSlot->Row - 1, 0, 3));
	}

	TArray<UWidget*> textWidgetArray = FindAllWidgets(UTextBlock::StaticClass());
	textWidgetArray[0]->RemoveFromParent();
	textWidgetArray.RemoveAt(0);
	for (UWidget* aWidget : textWidgetArray)
	{
		UTextBlock* textBlock = Cast<UTextBlock>(aWidget);
		UPanelSlot* panelSlot = textBlock->Slot;
		UUniformGridSlot* gridSlot = Cast<UUniformGridSlot>(panelSlot);
		gridSlot->SetRow(FMath::Clamp(gridSlot->Row - 1, 0, 3));
	}

	UWidget* theWidget = FindFirstWidget(UUniformGridPanel::StaticClass());
	UUniformGridPanel* thePanel = Cast<UUniformGridPanel>(theWidget);
	UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(thePanel->Slot);

	SetDesiredSizeInViewport(FVector2D(canvasSlot->GetSize().X, canvasSlot->GetSize().Y * 0.5));
	canvasSlot->SetSize(FVector2D(canvasSlot->GetSize().X, canvasSlot->GetSize().Y * 0.5));

	FAnchors anchorData = FAnchors();
	anchorData.Minimum = FVector2D(0.5, 1);
	anchorData.Maximum = FVector2D(0.5, 1);
	canvasSlot->SetAnchors(anchorData);
}

