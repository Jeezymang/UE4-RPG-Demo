// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "CustomUserWidgetParent.h"

FReply UCustomUserWidgetParent::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UWorld* theWorld = GetWorld();
	if (CanDrag && !IsDragging && theWorld && NextDragTime < theWorld->GetRealTimeSeconds())
	{
		APlayerController* plyCont = Cast<APlayerController>(GetOwningPlayerPawn()->GetController());
		if (plyCont && plyCont->IsInputKeyDown(EKeys::LeftShift))
		{
			wasMoved = true;
			NextDragTime = theWorld->GetTimeSeconds() + 1;
			TimeUntilMove = GetWorld()->GetTimeSeconds() + 0.5;
			IsDragging = true;
		}
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UCustomUserWidgetParent::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CanDrag && IsDragging)
	{
		IsDragging = false;
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UCustomUserWidgetParent::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (IsDragging && CanDrag)
	{
		UEngine* theEngine = GEngine;
		UWorld* theWorld = GetWorld();
		if (theWorld && theEngine && TimeUntilMove < theWorld->GetRealTimeSeconds()) {
			APlayerController* plyCont = Cast<APlayerController>(GetOwningPlayerPawn()->GetController());
			if (plyCont && !plyCont->IsInputKeyDown(EKeys::LeftMouseButton)){
				IsDragging = false;
			}
			FVector2D mousePosition = GetMousePos(theWorld);
			FVector2D localSize = MyGeometry.GetDrawSize();
			FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			mousePosition.X = FMath::Clamp(mousePosition.X - localSize.X*0.4, 0.0, viewportSize.X - localSize.X*0.75);
			mousePosition.Y = FMath::Clamp(mousePosition.Y - localSize.Y*0.4, 0.0, viewportSize.Y - localSize.Y*0.75);
			this->SetPositionInViewport(mousePosition);
		}
	}
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UCustomUserWidgetParent::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	m_MouseEnterDelegate.ExecuteIfBound();
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UCustomUserWidgetParent::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	m_MouseLeaveDelegate.ExecuteIfBound();
	Super::NativeOnMouseLeave(InMouseEvent);
}

FVector2D UCustomUserWidgetParent::GetMousePos(UWorld* theWorld)
{
	UEngine* theEngine = GEngine;
	if (theEngine && theWorld)
	{
		ULocalPlayer* LocalPlayer = theEngine->GetLocalPlayerFromControllerId(theWorld, 0);
		if (LocalPlayer)
		{
			FVector2D mousePosition = FVector2D(0, 0);
			bool isInWindow = LocalPlayer->ViewportClient->GetMousePosition(mousePosition);
			return mousePosition;
		}
	}
	return FVector2D(0, 0);
}

UWidget* UCustomUserWidgetParent::FindFirstWidget(UClass* widgetClass)
{
	UWidget* theWidget;
	WidgetTree->ForEachWidget([&](UWidget* Widget) {
		if (Widget->IsA(widgetClass))
		{
			theWidget = Widget;
		}	
	});
	return theWidget;
}

TArray<UWidget*> UCustomUserWidgetParent::FindAllWidgets(UClass* widgetClass)
{
	TArray<UWidget*> widgetArray;
	WidgetTree->ForEachWidget([&](UWidget* Widget) {
		if (Widget->IsA(widgetClass))
		{
			widgetArray.Add(Widget);
		}
	});
	return widgetArray;
}