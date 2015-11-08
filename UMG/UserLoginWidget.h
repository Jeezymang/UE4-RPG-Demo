// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "UserLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UUserLoginWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		UEditableTextBox* m_UsernameInput;

		UEditableTextBox* m_PasswordInput;

		bool m_IsAttemptingRequest;

		bool m_DisplayError;

		float m_ErrorLength;

		float m_ErrorEndTime;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FString m_FeedbackText;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
		FLinearColor m_TextColor;

		void SetupButtons();

		void TriggerErrorText(FString text, float length);

		UFUNCTION()
		void UserLogin();

		UFUNCTION()
		void UserRegister();

		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
