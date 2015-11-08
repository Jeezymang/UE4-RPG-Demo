// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UInventoryWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:
		
		void GenerateInventoryIcons();

		bool m_BoundItems;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
		FString ItemNameHint;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Details")
		FString ItemDescriptionHint;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
		int32 m_LineCharacterAmount;
};
