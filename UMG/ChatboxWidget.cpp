// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "ChatboxWidget.h"
#include "UMG/CustomButtonWidget.h"
#include "RPGDemoCharacter.h"
#include "UnrealNetwork.h"
#include <string>

void UChatboxWidget::SetupChatbox()
{
	UWidget* theWidget = FindFirstWidget(UCustomButtonWidget::StaticClass());
	if (theWidget)
	{
		UCustomButtonWidget* theButton = Cast<UCustomButtonWidget>(theWidget);
		theButton->FunctionDelegate.BindUObject(this, &UChatboxWidget::SendChat);
	}
}

void UChatboxWidget::SendChat()
{
	UWidget* theWidget = FindFirstWidget(UEditableTextBox::StaticClass());
	if (theWidget)
	{
		UEditableTextBox* theTextEntry = Cast<UEditableTextBox>(theWidget);
		FString theText = theTextEntry->GetText().ToString();
		if (theText == "") return;
		theTextEntry->SetText(FText::FromString(""));
		ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);
		if (theText.StartsWith("!savegamedata"))
			thePlayer->ServerSaveGameData(UStaticFunctions::UInt64ToFString(thePlayer->GetSteamID64()), thePlayer->GetPersonaName());
		else if (theText.StartsWith("!loadgamedata"))
			thePlayer->ServerLoadGameData(UStaticFunctions::UInt64ToFString(thePlayer->GetSteamID64()));
		else if (theText.StartsWith("!getquests")) 
		{
			for (FQuestDetails questDetails : thePlayer->m_Quests)
			{
				AddChat("Quest: " + questDetails.Name);
			}
		}
			
		if (thePlayer)
			thePlayer->ServerSendChatMessage(theText);
	}
}

void UChatboxWidget::AddChat(FString theText)
{
	UWidget* theTextWidget = FindFirstWidget(UTextBlock::StaticClass());
	if (theTextWidget)
	{
		UTextBlock* castedTextWidget = Cast<UTextBlock>(theTextWidget);
		castedTextWidget->SetText(FText::FromString(castedTextWidget->GetText().ToString() + "\n" + theText));
	}
	UWidget* theScrollWidget = FindFirstWidget(UScrollBox::StaticClass());
	if (theScrollWidget)
	{
		UScrollBox* castedScrollWidget = Cast<UScrollBox>(theScrollWidget);
		castedScrollWidget->SetScrollOffset(9999);
	}
}

