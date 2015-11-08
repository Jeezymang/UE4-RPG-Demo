// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "CustomButtonWidget.generated.h"

DECLARE_DELEGATE(FFunctionDelegate);

UCLASS()
class RPGDEMO_API UCustomButtonWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonDetails")
		FLinearColor DefaultButtonColor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonDetails")
		FLinearColor ButtonColor;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ButtonDetails")

		UTexture2D* ButtonImage = UStaticFunctions::LoadTextureFromPath("/Game/Textures/Icons/T_InvSlotEmpty");

		FFunctionDelegate FunctionDelegate;

		virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

		virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
