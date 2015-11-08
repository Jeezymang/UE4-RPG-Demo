// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "ChatboxWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UChatboxWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		void SetupChatbox();
		void SendChat();
		void AddChat(FString theText);

};
