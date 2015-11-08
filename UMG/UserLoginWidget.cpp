// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "UserLoginWidget.h"
#include "UMG/CustomButtonWidget.h"
#include "RPGDemoCharacter.h"

void UUserLoginWidget::SetupButtons()
{
	int32 buttonIndex = 0;
	int32 inputIndex = 0;
	WidgetTree->ForEachWidget([&](UWidget* Widget) {
		UCustomButtonWidget* buttonWidget = Cast<UCustomButtonWidget>(Widget);
		UEditableTextBox* inputWidget = Cast<UEditableTextBox>(Widget);
		if (buttonWidget) {
			buttonWidget->DefaultButtonColor = buttonWidget->ButtonColor;
			switch (buttonIndex)
			{
				case 0: // Login
					buttonWidget->FunctionDelegate.BindUObject(this, &UUserLoginWidget::UserLogin);
				break;
				case 1: // Register
					buttonWidget->FunctionDelegate.BindUObject(this, &UUserLoginWidget::UserRegister);
				break;
			}
			buttonIndex++;
		}
		else if (inputWidget) {
			switch (inputIndex)
			{
				case 0: //Username
					m_UsernameInput = inputWidget;
				break;
				case 1: //Password
					m_PasswordInput = inputWidget;
				break;
			}
			inputIndex++;
		}
	});
}

void UUserLoginWidget::UserLogin()
{
	if (m_IsAttemptingRequest) return;
	/*ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);
	if (!thePlayer) return;
	FString usernameText = m_UsernameInput->GetText().ToString().ToLower();
	FString passwordText = m_PasswordInput->GetText().ToString().ToLower();
	if (usernameText == "" || passwordText == "") return;
	thePlayer->ServerAttemptUserLogin(usernameText, passwordText);
	m_IsAttemptingRequest = true;*/
}

void UUserLoginWidget::UserRegister()
{
	if (m_IsAttemptingRequest) return;
	/*ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);
	if (!thePlayer) return;
	FString usernameText = m_UsernameInput->GetText().ToString().ToLower();
	FString passwordText = m_PasswordInput->GetText().ToString().ToLower();
	if (usernameText == "" || passwordText == "") return;
	FString ipAddress = UStaticFunctions::IntIPAddressToString(thePlayer->GetNetConnection()->GetAddrAsInt());
	thePlayer->ServerAttemptUserRegister(usernameText, passwordText, ipAddress);
	m_IsAttemptingRequest = true;*/
}

void UUserLoginWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UWorld* theWorld = GetWorld();
	float currentTime = theWorld->GetRealTimeSeconds();
	if (m_DisplayError)
	{
		if (currentTime > m_ErrorEndTime)
			m_DisplayError = false;
		else
		{
			float timeAlpha = ((m_ErrorEndTime - currentTime) / m_ErrorLength);
			m_TextColor = FLinearColor(1, 0, 0, timeAlpha);
		}
	}
	else
		m_TextColor = FLinearColor(0, 0, 0, 0);
}

void UUserLoginWidget::TriggerErrorText(FString text, float length)
{
	m_DisplayError = true;
	m_ErrorLength = length;
	m_ErrorEndTime = GetWorld()->GetRealTimeSeconds() + length;
	m_FeedbackText = text;
	m_IsAttemptingRequest = false;
}