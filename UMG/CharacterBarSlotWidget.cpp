// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "CharacterBarSlotWidget.h"

FReply UCharacterBarSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (FunctionDelegate.IsBound())
		FunctionDelegate.Execute();
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UCharacterBarSlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::OnMouseEnter(InGeometry, InMouseEvent);
}

void UCharacterBarSlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::OnMouseLeave(InMouseEvent);
}


