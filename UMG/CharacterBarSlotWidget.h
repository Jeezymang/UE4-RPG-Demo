// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "CharacterBarSlotWidget.generated.h"

DECLARE_DELEGATE(FFunctionDelegate);

UCLASS()
class RPGDEMO_API UCharacterBarSlotWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Info")
		FString SlotName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Info")
		UTexture2D* ItemTexture;

		FFunctionDelegate FunctionDelegate;

		virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
};
