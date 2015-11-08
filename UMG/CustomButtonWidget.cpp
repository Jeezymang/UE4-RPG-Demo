// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "CustomButtonWidget.h"

FReply UCustomButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (FunctionDelegate.IsBound())
		FunctionDelegate.Execute();
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UCustomButtonWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	ButtonColor = FLinearColor(0.45, 0.45, 0.45, 1);
}

void UCustomButtonWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	ButtonColor = DefaultButtonColor;
}


