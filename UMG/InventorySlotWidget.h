// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"

#include "InventorySlotWidget.generated.h"

UCLASS()
class RPGDEMO_API UInventorySlotWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		int32 ItemSlot;

		int32 ItemID;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		FString ItemName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		UTexture2D* ItemTexture;

		virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
