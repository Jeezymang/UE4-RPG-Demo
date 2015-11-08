// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "PlayerActionBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UPlayerActionBarWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		void SetupButtonFunctions();

		UCustomUserWidgetParent* m_AbilityInfoWidget;
		
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeyOneCooldownText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeyTwoCooldownText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeyThreeCooldownText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeyFourCooldownText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeyFiveCooldownText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeySixCooldownText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeySevenCooldownText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeyEightCooldownText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Info")
		FString m_ActionKeyNineCooldownText;

		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
