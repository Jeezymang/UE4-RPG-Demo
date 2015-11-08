// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"

class UInventoryWidget;

#include "InventoryOptionsWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UInventoryOptionsWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		int32 ItemSlot;

		int32 ItemID;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Details")
		FString ActionButtonText;

		void SetupButtonFunctions();

		void SetActionButtonText();

		void RemoveFirstButton();

		UFUNCTION()
		void ConsumeItem();

		UFUNCTION()
		void RemoveItem();

		UFUNCTION()
		void Cancel();
};
