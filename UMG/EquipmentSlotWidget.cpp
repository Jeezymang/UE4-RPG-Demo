// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "EquipmentSlotWidget.h"
#include "UMG/EquipmentWidget.h"
#include "RPGDemoCharacter.h"

FReply UEquipmentSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (FunctionDelegate.IsBound())
	{
		FunctionDelegate.Execute();
		if (m_ParentPanel) {
			ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_ParentPanel->m_OwningPlayer);
			if (thePlayer)
			{
				UEquipmentWidget* equipWidget = Cast<UEquipmentWidget>(thePlayer->m_Widgets.EquipmentWidget);
				equipWidget->GenerateEquipmentIcons();
			}
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UEquipmentSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UEquipmentWidget* parentPanel = Cast<UEquipmentWidget>(m_ParentPanel);
	if (!parentPanel) return Super::OnMouseEnter(InGeometry, InMouseEvent);

	if (ItemID != -1 && parentPanel)
	{
		parentPanel->m_NameHintText = ItemName;
		parentPanel->m_DescriptionHintText = ItemDescription;
	}
	Super::OnMouseEnter(InGeometry, InMouseEvent);
}

void UEquipmentSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	UEquipmentWidget* parentPanel = Cast<UEquipmentWidget>(m_ParentPanel);
	if (!parentPanel) return Super::OnMouseLeave(InMouseEvent);

	if (parentPanel)
	{
		parentPanel->m_NameHintText = "";
		parentPanel->m_DescriptionHintText = "";
	}
	Super::OnMouseLeave(InMouseEvent);
}


