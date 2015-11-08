// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "QuestLogPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UQuestLogPanelWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Details")
		FString m_QuestNameText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Details")
		FString m_QuestDescriptionText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Details")
		FString m_QuestTypeText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Details")
		FString m_QuestRequiredObjectText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Details")
		FString m_QuestProgressText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest Details")
		FString m_QuestRewardsText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
		FVector2D m_QuestSlotSize;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
		int32 m_LineCharacterAmount;

		FString m_SelectedQuest;

		void GenerateQuestSlots();
	
};
