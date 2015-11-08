// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "TargetPanelWidget.generated.h"

UCLASS()
class RPGDEMO_API UTargetPanelWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()

	public:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		float m_TargetHealthPercent;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		float m_TargetEnergyPercent;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		FString m_TargetHealthText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		FString m_TargetEnergyText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		FString m_TargetName;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		UTexture2D* m_TargetTexture;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target Stats")
		FString m_TargetLevel;

		AActor* m_TargetActor;

		int32 m_LastHealth;
		int32 m_LastEnergy;

		void SetTargetActor(AActor* theActor);
		void ClearTargetActor();

		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
