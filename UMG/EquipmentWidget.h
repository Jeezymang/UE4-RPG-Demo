// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "EquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UEquipmentWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		FString m_NameHintText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
		FString m_DescriptionHintText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString m_AttackPointsText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString m_DefensePointsText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString m_HealthPointsText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString m_EnergyPointsText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
		FString m_AttackSpeedText;

		UFUNCTION()
		void GenerateEquipmentIcons();
};
