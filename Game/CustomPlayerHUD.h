// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "CustomPlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API ACustomPlayerHUD : public AHUD
{
	GENERATED_BODY()
	
	public:
	
		TArray<FFloatingText> m_FloatingTextArray;
		void AddFloatingText(FString text, FLinearColor textColor, FString fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length);
		void AddFloatingTextEx(FString text, FLinearColor textColor, FString fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length, bool isRainbow, bool sizePulse, float sizePulseStrength, bool colorPulse, FLinearColor otherColor, bool horizontalPulse, float horizontalPulseStrength);
	
	protected:
		
		virtual void DrawHUD() override;
};
