// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "AbilityInfoWidget.h"

void UAbilityInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	SetPositionInViewport(GetMousePos(GetWorld()) - FVector2D(MyGeometry.GetLocalSize().X*0.25, 100));
	
}