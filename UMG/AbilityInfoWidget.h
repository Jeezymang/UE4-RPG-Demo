// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "AbilityInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UAbilityInfoWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:
	
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_AbilityNameText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_AbilityDescriptionText;

		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
