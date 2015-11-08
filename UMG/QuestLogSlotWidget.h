// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "QuestLogSlotWidget.generated.h"
/**
 * 
 */
UCLASS()
class RPGDEMO_API UQuestLogSlotWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Details")
		FString m_QuestNameText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Details")
		UTexture2D* m_ImageTexture;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		FLinearColor m_BorderHoverColor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		FLinearColor m_BorderDefaultColor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		FLinearColor m_BorderSelectedColor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		FLinearColor m_BorderColor;
		
		void GenerateQuestInfo();

		virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
};
