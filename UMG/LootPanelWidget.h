// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "LootPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API ULootPanelWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		AActor* m_OwningPlayer;

		AActor* m_OwningObject;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
		FVector2D m_LootSlotSize;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
		int32 m_LineCharacterAmount;

		void GenerateLootSlots();
};
