// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "LootSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API ULootSlotWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		int32 m_ItemID;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		FString m_ItemName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		FString m_ItemDescription;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		UTexture2D* m_ItemTexture;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		FLinearColor m_BorderHoverColor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		FLinearColor m_BorderDefaultColor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UMG")
		FLinearColor m_BorderColor;

		virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
