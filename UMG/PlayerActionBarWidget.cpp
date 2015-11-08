// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "PlayerActionBarWidget.h"
#include "RPGDemoCharacter.h"
#include "UMG/CustomButtonWidget.h"
#include "UMG/AbilityInfoWidget.h"

void UPlayerActionBarWidget::SetupButtonFunctions()
{
	int32 index = 0;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);
	if (!thePlayer) return;
	WidgetTree->ForEachWidget([&](UWidget* Widget) {
		UCustomButtonWidget* buttonWidget = Cast<UCustomButtonWidget>(Widget);
		if (buttonWidget){
			buttonWidget->FunctionDelegate.BindUObject(thePlayer, &ARPGDemoCharacter::ServerPlayerUseAction, index);
			//	m_MouseEnt
			EPlayerAbilities abilityEnum = UStaticFunctions::IndexToActionSlot(index, thePlayer);
			bool hasAbilityInSlot = false;
			if (abilityEnum != EPlayerAbilities::None)
				hasAbilityInSlot = true;
			if (hasAbilityInSlot)
			{
				FAbilityData abilityStruct = (FAbilityData)UStaticFunctions::m_AbilityMap[abilityEnum];
				buttonWidget->ButtonImage = UStaticFunctions::LoadTextureFromPath(abilityStruct.TexturePath);
				buttonWidget->m_MouseEnterDelegate.BindLambda([=] {
					if (!m_AbilityInfoWidget)
					{
						m_AbilityInfoWidget = CreateWidget<UCustomUserWidgetParent>(thePlayer->Controller->CastToPlayerController(), thePlayer->m_WidgetClasses.AbilityInfoWidget);
						m_AbilityInfoWidget->AddToViewport(0);
					}
					m_AbilityInfoWidget->SetPositionInViewport(GetMousePos(GetWorld()) - FVector2D(0, 100));
					UAbilityInfoWidget* abilityInfoWidget = Cast<UAbilityInfoWidget>(m_AbilityInfoWidget);
					abilityInfoWidget->m_AbilityNameText = abilityStruct.Name;
					abilityInfoWidget->m_AbilityDescriptionText = abilityStruct.Description;
				});
				buttonWidget->m_MouseLeaveDelegate.BindLambda([=] {
					if (m_AbilityInfoWidget)
						m_AbilityInfoWidget->RemoveFromViewport();
					m_AbilityInfoWidget = NULL;
				});
			}
			else
				buttonWidget->ButtonImage = UStaticFunctions::LoadTextureFromPath("/Game/Textures/Icons/T_ActionSlotEmpty");
			index++;
		}
	});
}

void UPlayerActionBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_OwningPlayer);
	UWorld* theWorld = GetWorld();
	if (!thePlayer) return;
	if (!theWorld) return;

	float currentTime = theWorld->GetRealTimeSeconds();

	for (int i = 0; i < 9; i++)
	{
		EPlayerAbilities abilityEnum = UStaticFunctions::IndexToActionSlot(i, thePlayer);
		if (abilityEnum == EPlayerAbilities::None)
			continue;

		bool clearCooldownText = false;
		float timeRemaining = 0;
		if (!thePlayer->IsAbilityOnCooldown(abilityEnum))
			clearCooldownText = true;
		else
			timeRemaining = thePlayer->m_AbilityCooldowns[abilityEnum] - currentTime;

		FString cooldownText = "\n";
		switch (i)
		{
			case 0:
				if (clearCooldownText)
					m_ActionKeyOneCooldownText = "";
				else
					m_ActionKeyOneCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
			case 1:
				if (clearCooldownText)
					m_ActionKeyTwoCooldownText = "";
				else
					m_ActionKeyTwoCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
			case 2:
				if (clearCooldownText)
					m_ActionKeyThreeCooldownText = "";
				else
					m_ActionKeyThreeCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
			case 3:
				if (clearCooldownText)
					m_ActionKeyFourCooldownText = "";
				else
					m_ActionKeyFourCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
			case 4:
				if (clearCooldownText)
					m_ActionKeyFiveCooldownText = "";
				else
					m_ActionKeyFiveCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
			case 5:
				if (clearCooldownText)
					m_ActionKeySixCooldownText = "";
				else
					m_ActionKeySixCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
			case 6:
				if (clearCooldownText)
					m_ActionKeySevenCooldownText = "";
				else
					m_ActionKeySevenCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
			case 7:
				if (clearCooldownText)
					m_ActionKeyEightCooldownText = "";
				else
					m_ActionKeyEightCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
			case 8:
				if (clearCooldownText)
					m_ActionKeyNineCooldownText = "";
				else
					m_ActionKeyNineCooldownText = cooldownText + FString::FromInt((int)timeRemaining) + "s";
			break;
		}
	}
}

