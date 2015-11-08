// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "PlayerVitalsWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UPlayerVitalsWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		float m_HealthPercent;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		float m_EnergyPercent;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		FString m_HealthText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		FString m_EnergyText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		FString m_Level;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		FString m_ExperienceText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		float m_LevelPercent;
	
		int32 m_LastHealth;
		int32 m_LastEnergy;

		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
