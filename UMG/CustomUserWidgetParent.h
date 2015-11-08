// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CustomUserWidgetParent.generated.h"

DECLARE_DELEGATE(FMouseEnterDelegate);
DECLARE_DELEGATE(FMouseLeaveDelegate);
/**
 * 
 */
UCLASS()
class RPGDEMO_API UCustomUserWidgetParent : public UUserWidget
{
	GENERATED_BODY()
	
	public:

		AActor* m_OwningPlayer;

		UCustomUserWidgetParent* m_ParentPanel;

		bool IsDragging;

		float NextDragTime;

		float TimeUntilMove;

		float NextClickToDrag;

		FVector2D LastClickPos;

		bool wasMoved;

		FMouseEnterDelegate m_MouseEnterDelegate;

		FMouseLeaveDelegate m_MouseLeaveDelegate;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Widget")
		bool CanDrag;

		FVector2D GetMousePos(UWorld* theWorld);

		virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

		virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

		UWidget* FindFirstWidget(UClass* widgetClass);

		TArray<UWidget*> FindAllWidgets(UClass* widgetClass);
};
