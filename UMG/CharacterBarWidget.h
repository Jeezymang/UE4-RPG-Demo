// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UMG/CustomUserWidgetParent.h"
#include "CharacterBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class RPGDEMO_API UCharacterBarWidget : public UCustomUserWidgetParent
{
	GENERATED_BODY()
	
	public:

		void SetupActions();
};
