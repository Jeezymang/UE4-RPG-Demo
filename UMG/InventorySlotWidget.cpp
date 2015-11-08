// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "InventorySlotWidget.h"
#include "UMG/InventoryWidget.h"
#include "UMG/InventoryOptionsWidget.h"
#include "RPGDemoCharacter.h"

void UInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UStaticFunctions::ItemIDExists(ItemID)) {
		UInventoryWidget* parentPanel = Cast<UInventoryWidget>(m_ParentPanel);
		if (!parentPanel) return Super::OnMouseEnter(InGeometry, InMouseEvent);
		FItemDetails itemStruct = UStaticFunctions::m_ItemMap[ItemID];
		parentPanel->ItemNameHint = itemStruct.ItemName;
		parentPanel->ItemDescriptionHint = UStaticFunctions::SplitStringToLines(itemStruct.ItemDescription, parentPanel->m_LineCharacterAmount);
	}
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UInventoryWidget* parentPanel = Cast<UInventoryWidget>(m_ParentPanel);
	if (!parentPanel) return Super::OnMouseLeave(InMouseEvent);
	parentPanel->ItemNameHint = "";
	parentPanel->ItemDescriptionHint = "";
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) {
	if (ItemID != -1) {
		if (!m_ParentPanel->m_OwningPlayer) return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
		ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_ParentPanel->m_OwningPlayer);
		if (!thePlayer) return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
		if (thePlayer->m_Widgets.InventoryOptionsWidget) {
			thePlayer->m_Widgets.InventoryOptionsWidget->RemoveFromViewport();
			thePlayer->m_Widgets.InventoryOptionsWidget = NULL;
		}
		UCustomUserWidgetParent* optionWidget = CreateWidget<UCustomUserWidgetParent>(thePlayer->Controller->CastToPlayerController(), thePlayer->m_WidgetClasses.InventoryOptionsWidget);
		optionWidget->AddToViewport(0);
		thePlayer->m_Widgets.InventoryOptionsWidget = optionWidget;
		UInventoryOptionsWidget* castedOptionWidget = Cast<UInventoryOptionsWidget>(optionWidget);
		castedOptionWidget->ItemSlot = ItemSlot;
		castedOptionWidget->ItemID = ItemID;
		castedOptionWidget->m_ParentPanel = m_ParentPanel;
		castedOptionWidget->SetupButtonFunctions();
		if (GetWorld()) {
			ULocalPlayer* LocalPlayer = GEngine->GetLocalPlayerFromControllerId(GetWorld(), 0);
			if (LocalPlayer && LocalPlayer->ViewportClient) {
				FVector2D mousePosition = FVector2D(0, 0);
				bool isInWindow = LocalPlayer->ViewportClient->GetMousePosition(mousePosition);
				if (isInWindow)
					optionWidget->SetPositionInViewport(mousePosition);
			}
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}