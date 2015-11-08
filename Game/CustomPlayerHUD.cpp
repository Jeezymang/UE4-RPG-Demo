// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "CustomPlayerHUD.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/RendererSettings.h"

void ACustomPlayerHUD::AddFloatingText(FString text, FLinearColor textColor, FString fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length)
{
	FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	FFloatingText textStruct = FFloatingText();
	textStruct.Text = text;
	textStruct.TextColor = textColor;
	textStruct.TextFont = UStaticFunctions::LoadFontFromPath(fontPath);
	textStruct.TextScale = textScale;
	textStruct.StartPos = FVector2D(-(viewportSize.X * 0.5) + viewportSize.X * startPos.X, -(viewportSize.Y * 0.5) + viewportSize.Y * startPos.Y);
	textStruct.PosOffset = posOffset;
	textStruct.StartTime = GetWorld()->GetRealTimeSeconds();
	textStruct.EndTime = GetWorld()->GetRealTimeSeconds() + length;
	textStruct.IsRainbow = false;
	textStruct.SizePulse = false;
	textStruct.ColorPulse = false;
	textStruct.HorizontalPulse = false;
	m_FloatingTextArray.Add(textStruct);
}

void ACustomPlayerHUD::AddFloatingTextEx(FString text, FLinearColor textColor, FString fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length, bool isRainbow, bool sizePulse, float sizePulseStrength, bool colorPulse, FLinearColor otherColor, bool horizontalPulse, float horizontalPulseStrength)
{
	FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	FFloatingText textStruct = FFloatingText();
	textStruct.Text = text;
	textStruct.TextColor = textColor;
	textStruct.TextFont = UStaticFunctions::LoadFontFromPath(fontPath);
	textStruct.TextScale = textScale;
	textStruct.StartPos = FVector2D(-(viewportSize.X * 0.5) + viewportSize.X * startPos.X, -(viewportSize.Y * 0.5) + viewportSize.Y * startPos.Y);
	textStruct.PosOffset = posOffset;
	textStruct.StartTime = GetWorld()->GetRealTimeSeconds();
	textStruct.EndTime = GetWorld()->GetRealTimeSeconds() + length;
	textStruct.IsRainbow = isRainbow;
	textStruct.SizePulse = sizePulse;
	textStruct.SizePulseStrength = sizePulseStrength;
	textStruct.ColorPulse = colorPulse;
	textStruct.OtherColor = otherColor;
	textStruct.HorizontalPulse = horizontalPulse;
	textStruct.HorizontalPulseStrength = horizontalPulseStrength;
	m_FloatingTextArray.Add(textStruct);
}

void ACustomPlayerHUD::DrawHUD()
{
	if (!GetWorld()) return;
	TArray<int32> m_IndexesToRemove;
	FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	float viewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));
	for (int32 i = 0; i < m_FloatingTextArray.Num(); i++)
	{
		FFloatingText textStruct = m_FloatingTextArray[i];
		float currentTime = GetWorld()->GetRealTimeSeconds();
		float totalLength = textStruct.EndTime - textStruct.StartTime;
		float timeAlpha = 1 - ((textStruct.EndTime - currentTime) / totalLength);
		FVector2D theScale = textStruct.TextScale;
		float textWidth, textHeight;
		GetTextSize(textStruct.Text, textWidth, textHeight, textStruct.TextFont, theScale.X);
		FVector2D currentPos = FMath::Lerp(textStruct.StartPos - FVector2D((textWidth * 0.5) * viewportScale, textHeight * viewportScale), (textStruct.StartPos - FVector2D((textWidth * 0.5) * viewportScale, textHeight * viewportScale)) + textStruct.PosOffset, timeAlpha);
		FLinearColor theColor = textStruct.TextColor;
		if (textStruct.IsRainbow && !textStruct.ColorPulse)
			theColor = UStaticFunctions::RainbowStrobe(GetWorld(), 5, 1);
		if (textStruct.SizePulse)
		{
			float scaleAddition = FMath::Sin(currentTime * 3) * textStruct.SizePulseStrength;
			theScale = FVector2D(theScale.X + scaleAddition, theScale.Y + scaleAddition);
		}
		if (textStruct.ColorPulse && !textStruct.IsRainbow)
		{
			FLinearColor newColor = FLinearColor::LerpUsingHSV(theColor, textStruct.OtherColor, FMath::Clamp(FMath::Sin(currentTime * 3), 0.0f, 1.0f));
			theColor = newColor;
		}
		if (textStruct.HorizontalPulse)
		{
			float xOffset = FMath::Sin(currentTime * 3) * textStruct.HorizontalPulseStrength;
			currentPos.X += xOffset;
		}
		theColor.A = 1 - timeAlpha;
		
		DrawText(textStruct.Text, currentPos, textStruct.TextFont, theScale, theColor.ToFColor(true));
		if (textStruct.EndTime < currentTime)
		{
			if (!m_IndexesToRemove.Contains(i))
				m_IndexesToRemove.Add(i);
		}
	}
	for (int32 index : m_IndexesToRemove)
	{
		if (m_FloatingTextArray.IsValidIndex(index))
			m_FloatingTextArray.RemoveAt(index);
	}
}

