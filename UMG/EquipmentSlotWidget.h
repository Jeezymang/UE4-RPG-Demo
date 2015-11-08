// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"

class UEquipmentWidget;

#include "EquipmentSlotWidget.generated.h"

DECLARE_DELEGATE(FFunctionDelegate);

UCLASS()
class RPGDEMO_API UEquipmentSlotWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		int32 SlotNumber;

		int32 ItemID;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Details")
		EEquipmentSlot SlotType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		FString ItemName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Description")
		FString ItemDescription;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		UTexture2D* ItemTexture;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slot Details")
		FLinearColor SlotColor;

		FFunctionDelegate FunctionDelegate;

		virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
