// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#include "RPGDemo.h"
#include "RPGDemoCharacter.h"
#include "UnrealNetwork.h"
#include "UMG/TargetPanelWidget.h"
#include "UMG/EquipmentWidget.h"
#include "UMG/InventoryWidget.h"
#include "UMG/CharacterBarWidget.h"
#include "UMG/ChatboxWidget.h"
#include "UMG/UserLoginWidget.h"
#include "UMG/PlayerActionBarWidget.h"
#include "UMG/LootPanelWidget.h"
#include "UMG/PlayerVitalsWidget.h"
#include "UMG/QuestLogPanelWidget.h"
#include "Game/ItemPickupActorParent.h"
#include "Game/WornEquipmentActorParent.h"
#include "Game/CustomPlayerHUD.h"
#include "Game/LootableActorParent.h"
#include "Game/StaticPlayerAbilities.h"
#include "Json.h"
#include "Regex.h"
#include "Bots/BotCharacterParent.h"
#include "ThirdParty/Steamworks/Steamv132/sdk/public/steam/steam_api.h"
#include "ThirdParty/Steamworks/Steamv132/sdk/public/steam/isteamfriends.h"
#include "ThirdParty/Steamworks/Steamv132/sdk/public/steam/isteamutils.h"

//////////////////////////////////////////////////////////////////////////
// ARPGDemoCharacter

ARPGDemoCharacter::ARPGDemoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// GetLifetimeReplicatedProps
void ARPGDemoCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARPGDemoCharacter, m_Health);
	DOREPLIFETIME(ARPGDemoCharacter, m_MaxHealth);
	DOREPLIFETIME(ARPGDemoCharacter, m_Energy);
	DOREPLIFETIME(ARPGDemoCharacter, m_InCombat);
	DOREPLIFETIME(ARPGDemoCharacter, m_IsDead);
	DOREPLIFETIME(ARPGDemoCharacter, m_IsStunned);
	DOREPLIFETIME(ARPGDemoCharacter, m_DisplayName);
	DOREPLIFETIME(ARPGDemoCharacter, m_LegacySteamID);
	DOREPLIFETIME(ARPGDemoCharacter, m_BodyColor);
	DOREPLIFETIME(ARPGDemoCharacter, m_Equipment);
	DOREPLIFETIME(ARPGDemoCharacter, m_Level);
	//DOREPLIFETIME_CONDITION(ARPGDemoCharacter, m_Equipment, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ARPGDemoCharacter, m_Inventory, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ARPGDemoCharacter, m_Experience, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ARPGDemoCharacter, m_Target, COND_OwnerOnly);
}


//////////////////////////////////////////////////////////////////////////
// Begin Play
void ARPGDemoCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(m_DisplayName != "")
		Tags.Add("Targetable");
	m_BodyMaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	FCommandDelegate giveItemDelegate;
	giveItemDelegate.BindLambda([=](FString argText) {
		int32 itemID = UStaticFunctions::GetItemIDByName(argText);
		if (!UStaticFunctions::ItemIDExists(itemID))
			ClientDisplayErrorMessage(argText + " is not a valid item");
		else
		{
			ClientDisplayNotifyMessage("Giving a " + argText + "!");
			m_Inventory.Add(itemID);
		}
	});
	m_ServerCommands.Add("!giveitem", giveItemDelegate);
	FCommandDelegate giveQuestDelegate;
	giveQuestDelegate.BindLambda([=](FString argText) {
		if (!UStaticFunctions::m_QuestMap.Contains(argText))
			ClientDisplayErrorMessage(argText + " is not a valid quest.");
		else if (HasQuest(argText))
			ClientDisplayErrorMessage("You already have the quest " + argText + ".");
		else
		{
			ClientDisplayNotifyMessage("Giving Quest " + argText + ".");
			GiveQuest(argText);
		}
	});
	m_ServerCommands.Add("!givequest", giveQuestDelegate);
}

void ARPGDemoCharacter::Destroyed()
{
	Super::Destroyed();
	if (m_Equipment.HeadSlot)
		m_Equipment.HeadSlot->Destroy();
	if (m_Equipment.BackSlot)
		m_Equipment.BackSlot->Destroy();
	if (m_Equipment.ChestSlot)
		m_Equipment.ChestSlot->Destroy();
	if (m_Equipment.RightShoulderSlot)
		m_Equipment.RightShoulderSlot->Destroy();
	if (m_Equipment.LeftShoulderSlot)
		m_Equipment.LeftShoulderSlot->Destroy();
	if (m_Equipment.RightHandSlot)
		m_Equipment.RightHandSlot->Destroy();
	if (m_Equipment.LeftHandSlot)
		m_Equipment.LeftHandSlot->Destroy();

	//holy shit ghetto check
	if (!IsLocallyControlled() && m_Steam64 != "")
	{
		FPlayerData playerData = FPlayerData();
		playerData.DisplayName = m_DisplayName;
		playerData.Rank = "user";
		playerData.Level = m_Level;
		playerData.Experience = m_Experience;
		TArray<int32> equipmentArray = UStaticFunctions::GetEquipmentArray(m_Equipment);
		if (m_Steam64 == "0")
		{
			UStaticFunctions::DebugMessage(FColor::Red, "Couldn't get player's Steam64");
		}
		else
		{
			FBufferArchive toBinary;
			UStaticFunctions::SaveGameDataToPath("E:/TestData/" + m_Steam64 + ".dat", toBinary, playerData, m_Inventory, equipmentArray);
		}
	}
}

void ARPGDemoCharacter::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	Super::EndPlay(endPlayReason);
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPGDemoCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &ARPGDemoCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ARPGDemoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ARPGDemoCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ARPGDemoCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ARPGDemoCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ARPGDemoCharacter::TouchStopped);

	// Additional input
	InputComponent->BindAction("MouseWheelUp", IE_Pressed, this, &ARPGDemoCharacter::MouseWheelUp);
	InputComponent->BindAction("MouseWheelDown", IE_Pressed, this, &ARPGDemoCharacter::MouseWheelDown);

	InputComponent->BindAction("LeftControl", IE_Pressed, this, &ARPGDemoCharacter::PressedLeftControl);

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &ARPGDemoCharacter::PressedLeftClick);
	InputComponent->BindAction("LeftClick", IE_Released, this, &ARPGDemoCharacter::ReleasedLeftClick);

	InputComponent->BindAction("RightClick", IE_Pressed, this, &ARPGDemoCharacter::PressedRightClick);
	InputComponent->BindAction("RightClick", IE_Released, this, &ARPGDemoCharacter::ReleasedRightClick);

	//Action Keys
	TArray<FName> actionKeys;
	FName actionKeyNames[] = {FName("ActionKeyOne"), FName("ActionKeyTwo"), FName("ActionKeyThree"), FName("ActionKeyFour"), FName("ActionKeyFive"),
		FName("ActionKeySix"), FName("ActionKeySeven"), FName("ActionKeyEight"), FName("ActionKeyNine") };
	actionKeys.Append(actionKeyNames, ARRAYSIZE(actionKeyNames));
	for (int i = 0; i < actionKeys.Num(); i++)
	{
		FInputActionBinding newBinding = FInputActionBinding();
		newBinding.KeyEvent = IE_Pressed;
		newBinding.ActionName = actionKeyNames[i];
		newBinding.ActionDelegate.GetDelegateForManualSet().BindUObject(this, &ARPGDemoCharacter::PressedActionKey, i);
		InputComponent->AddActionBinding(newBinding);
	}
}

void ARPGDemoCharacter::PostNetInit()
{
	Super::PostNetInit();
	if (!Controller) return;
	UStaticFunctions::InitializeData();
	ServerSendSteamData(UStaticFunctions::UInt64ToFString(GetSteamID64()), GetLegacySteamID(), GetPersonaName(), UStaticFunctions::IntIPAddressToString(GetWorld()->NetDriver->ServerConnection->GetAddrAsInt()));
	APlayerController* playerController = Controller->CastToPlayerController();
	playerController->bShowMouseCursor = true;
	CreatePlayerWidget(m_WidgetClasses.TargetWidget, m_Widgets.TargetWidget, ESlateVisibility::Hidden);
	UEquipmentWidget* equipmentWidget = Cast<UEquipmentWidget>(CreatePlayerWidget(m_WidgetClasses.EquipmentWidget, m_Widgets.EquipmentWidget,  ESlateVisibility::Hidden));
	equipmentWidget->GenerateEquipmentIcons();
	UInventoryWidget* inventoryWidget = Cast<UInventoryWidget>(CreatePlayerWidget(m_WidgetClasses.InventoryWidget, m_Widgets.InventoryWidget, ESlateVisibility::Hidden));
	inventoryWidget->GenerateInventoryIcons();
	UQuestLogPanelWidget* questLogWidget = Cast<UQuestLogPanelWidget>(CreatePlayerWidget(m_WidgetClasses.QuestLogWidget, m_Widgets.QuestLogWidget, ESlateVisibility::Hidden));
	questLogWidget->GenerateQuestSlots();
	UCharacterBarWidget* characterBarWidget = Cast<UCharacterBarWidget>(CreatePlayerWidget(m_WidgetClasses.CharacterBarWidget, m_Widgets.CharacterBarWidget, ESlateVisibility::Visible));
	characterBarWidget->SetupActions();
	UChatboxWidget* chatboxWidget = Cast<UChatboxWidget>(CreatePlayerWidget(m_WidgetClasses.ChatboxWidget, m_Widgets.ChatboxWidget, ESlateVisibility::Visible));
	chatboxWidget->SetupChatbox();
	UPlayerActionBarWidget* actionBarWidget = Cast<UPlayerActionBarWidget>(CreatePlayerWidget(m_WidgetClasses.PlayerActionBarWidget, m_Widgets.PlayerActionBarWidget, ESlateVisibility::Visible));
	actionBarWidget->SetupButtonFunctions();
	UPlayerVitalsWidget* vitalsWidget = Cast<UPlayerVitalsWidget>(CreatePlayerWidget(m_WidgetClasses.PlayerVitalsWidget, m_Widgets.PlayerVitalsWidget, ESlateVisibility::Visible));
	vitalsWidget->m_LastHealth = m_Health;
	vitalsWidget->m_LastEnergy = m_Energy;
}

void ARPGDemoCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ARPGDemoCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ARPGDemoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ARPGDemoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ARPGDemoCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !m_IsStunned)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ARPGDemoCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && !m_IsStunned )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ARPGDemoCharacter::Jump()
{
	if (!m_IsStunned)
		Super::Jump();
}

void ARPGDemoCharacter::PressedLeftClick()
{
	AActor* clickedActor = GetClickedActor();
	if (clickedActor)
		ServerSetTarget(clickedActor);
	else
		ServerSetTarget(NULL);
}

void ARPGDemoCharacter::ReleasedLeftClick()
{
}

void ARPGDemoCharacter::PressedRightClick()
{
	if (m_Target)
	{
		AWornEquipmentActorParent* theItem = UStaticFunctions::GetEquipmentSlot(this, EEquipmentSlot::RightHand);
		FWeaponStats theStats = UStaticFunctions::GetWeaponStats(theItem);
		GetWorld()->GetTimerManager().ClearTimer(m_CombatHandle);
		GetWorld()->GetTimerManager().SetTimer(m_CombatHandle, this, &ARPGDemoCharacter::OnCombatTimerTick, theStats.AttackSpeed, true);
		ServerSetCombatStatus(true);
	}
}

void ARPGDemoCharacter::ReleasedRightClick()
{
}

void ARPGDemoCharacter::PressedLeftControl()
{
	if (m_FirstPersonToggled) 
	{
		CameraBoom->TargetArmLength = m_LastCameraArmLength;
		CameraBoom->SetRelativeLocation(FVector(0, 0, 21.271957f));
		bUseControllerRotationYaw = false;
		m_FirstPersonToggled = false;
	}
	else 
	{
		m_LastCameraArmLength = CameraBoom->TargetArmLength;
		CameraBoom->TargetArmLength = 0;
		CameraBoom->SetRelativeLocation(FVector(29.999952f, 0, 61.271706f));
		bUseControllerRotationYaw = true;
		m_FirstPersonToggled = true;
	}
}

void ARPGDemoCharacter::MouseWheelUp()
{
	if (m_FirstPersonToggled) return;
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - m_ScrollSpeed, m_MinScrollAmount, m_MaxScrollAmount);
	m_LastCameraArmLength = CameraBoom->TargetArmLength;
}

void ARPGDemoCharacter::MouseWheelDown()
{
	if (m_FirstPersonToggled) return;
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + m_ScrollSpeed, m_MinScrollAmount, m_MaxScrollAmount);
	m_LastCameraArmLength = CameraBoom->TargetArmLength;
}

void ARPGDemoCharacter::PressedActionKey(int slot)
{
	ServerPlayerUseAction(slot);
}

void ARPGDemoCharacter::ParseServerCommand(FString commandText)
{
	for (auto It = m_ServerCommands.CreateConstIterator(); It; ++It)
	{
		if (commandText.StartsWith(It.Key()))
		{
			FString replaceText = It.Key() + " ";
			FString argText = commandText.Replace(*replaceText, TEXT(""));
			It.Value().ExecuteIfBound(argText);
			break;
		}
	}
}

UCustomUserWidgetParent* ARPGDemoCharacter::CreatePlayerWidget(UClass* widgetClass, UCustomUserWidgetParent*& outVar, ESlateVisibility visibility)
{
	APlayerController* playerController = Controller->CastToPlayerController();
	UCustomUserWidgetParent* aWidget = CreateWidget<UCustomUserWidgetParent>(playerController, widgetClass);
	outVar = aWidget;
	aWidget->AddToViewport(0);
	aWidget->m_OwningPlayer = this;
	FPlayerUMGPosition positionData = UStaticFunctions::GetDefinedUMGPosition(widgetClass, m_WidgetPositions);
	if (positionData.WidgetClass)
	{
		FViewportData viewportData = UStaticFunctions::GetViewportData();
		aWidget->SetPositionInViewport(FVector2D((viewportData.ViewportSize.X * positionData.ViewportRatio.X) + (positionData.ViewportOffset.X * viewportData.ViewportScale), (viewportData.ViewportSize.Y * positionData.ViewportRatio.Y) + (positionData.ViewportOffset.Y*viewportData.ViewportScale)));
	}
	if(visibility != ESlateVisibility::Visible)
		aWidget->SetVisibility(visibility);
	return aWidget;
}

void ARPGDemoCharacter::AttachEquipment(int32 itemID)
{
	if (!UStaticFunctions::m_EquipmentClassMap.Contains(itemID)) return;
	AActor* spawnedActor = GetWorld()->SpawnActor(m_WornEquipClass);
	AWornEquipmentActorParent* theEquipment = Cast<AWornEquipmentActorParent>(spawnedActor);
	if (theEquipment && IsEquipmentSlotEmpty(UStaticFunctions::GetEquipmentSlotByID(itemID)))
	{
		FWornEquipmentData equipData = UStaticFunctions::m_WornEquipmentDataMap[itemID];
		theEquipment->m_SocketName = equipData.SocketName;
		theEquipment->m_Offset = equipData.Offset;
		theEquipment->m_Rotation = equipData.Rotation;
		theEquipment->m_Scale = equipData.Scale;
		theEquipment->m_ItemName = UStaticFunctions::m_ItemMap[itemID].ItemName;
		FVector socketLocation = GetMesh()->GetSocketLocation(equipData.SocketName);
		theEquipment->m_StaticMesh->SetStaticMesh(UStaticFunctions::LoadStaticMeshFromPath(UStaticFunctions::m_EquipmentClassMap[itemID]));
		spawnedActor->SetActorRelativeRotation(GetMesh()->GetSocketRotation(equipData.SocketName));
		spawnedActor->AddActorLocalRotation(FQuat::MakeFromEuler(equipData.Rotation));
		spawnedActor->SetActorRelativeLocation(socketLocation);
		spawnedActor->AddActorLocalOffset(equipData.Offset);
		spawnedActor->GetRootComponent()->SetWorldScale3D(equipData.Scale);
		spawnedActor->SetActorEnableCollision(false);
		theEquipment->m_StaticMesh->bGenerateOverlapEvents = false;
		theEquipment->m_StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		spawnedActor->GetRootComponent()->AttachTo(GetMesh(), equipData.SocketName, EAttachLocation::KeepWorldPosition);
		EquipSlot(theEquipment);
	}
	else if (spawnedActor && !IsEquipmentSlotEmpty(UStaticFunctions::GetEquipmentSlotByID(itemID)))
	{
		spawnedActor->Destroy();
	}
}

void ARPGDemoCharacter::ToggleEquipmentPanel()
{
	if (!m_Widgets.EquipmentWidget) return;
	if (m_Widgets.EquipmentWidget->GetVisibility() == ESlateVisibility::Visible)
		m_Widgets.EquipmentWidget->SetVisibility(ESlateVisibility::Hidden);
	else
		m_Widgets.EquipmentWidget->SetVisibility(ESlateVisibility::Visible);
}

void ARPGDemoCharacter::ToggleInventoryPanel()
{
	if (!m_Widgets.InventoryWidget) return;
	if (m_Widgets.InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
	{
		if (m_Widgets.QuestLogWidget && !m_Widgets.InventoryWidget->wasMoved && !m_Widgets.QuestLogWidget->wasMoved)
		{
			FPlayerUMGPosition questLogPosition = UStaticFunctions::GetDefinedUMGPosition(m_WidgetClasses.QuestLogWidget, m_WidgetPositions);
			FPlayerUMGPosition inventoryPosition = UStaticFunctions::GetDefinedUMGPosition(m_WidgetClasses.InventoryWidget, m_WidgetPositions);
			FViewportData viewportData = UStaticFunctions::GetViewportData();
			if (m_Widgets.QuestLogWidget->GetVisibility() == ESlateVisibility::Visible)
			{
				m_Widgets.QuestLogWidget->SetPositionInViewport(FVector2D((viewportData.ViewportSize.X * inventoryPosition.ViewportRatio.X) + ((questLogPosition.ViewportOffset.X - inventoryPosition.ViewportOffset.X) * viewportData.ViewportScale), (viewportData.ViewportSize.Y * inventoryPosition.ViewportRatio.Y) + ((questLogPosition.ViewportOffset.Y - inventoryPosition.ViewportOffset.Y)*viewportData.ViewportScale)));
			}
		}
		m_Widgets.InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (m_Widgets.QuestLogWidget && !m_Widgets.InventoryWidget->wasMoved && !m_Widgets.QuestLogWidget->wasMoved)
		{
			FPlayerUMGPosition questLogPosition = UStaticFunctions::GetDefinedUMGPosition(m_WidgetClasses.QuestLogWidget, m_WidgetPositions);
			FViewportData viewportData = UStaticFunctions::GetViewportData();
			if (m_Widgets.QuestLogWidget->GetVisibility() == ESlateVisibility::Visible)
			{
				m_Widgets.QuestLogWidget->SetPositionInViewport(FVector2D((viewportData.ViewportSize.X * questLogPosition.ViewportRatio.X) + (questLogPosition.ViewportOffset.X * viewportData.ViewportScale), (viewportData.ViewportSize.Y * questLogPosition.ViewportRatio.Y) + (questLogPosition.ViewportOffset.Y*viewportData.ViewportScale)));
			}
		}
		m_Widgets.InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	}
		
}

void ARPGDemoCharacter::ToggleQuestLogPanel()
{
	if (!m_Widgets.QuestLogWidget) return;
	if (m_Widgets.QuestLogWidget->GetVisibility() == ESlateVisibility::Visible)
		m_Widgets.QuestLogWidget->SetVisibility(ESlateVisibility::Hidden);
	else
	{
		if (m_Widgets.InventoryWidget && !m_Widgets.InventoryWidget->wasMoved && !m_Widgets.QuestLogWidget->wasMoved)
		{
			FPlayerUMGPosition questLogPosition = UStaticFunctions::GetDefinedUMGPosition(m_WidgetClasses.QuestLogWidget, m_WidgetPositions);
			FPlayerUMGPosition inventoryPosition = UStaticFunctions::GetDefinedUMGPosition(m_WidgetClasses.InventoryWidget, m_WidgetPositions);
			FViewportData viewportData = UStaticFunctions::GetViewportData();
			if (m_Widgets.InventoryWidget->GetVisibility() == ESlateVisibility::Hidden)
			{
				m_Widgets.QuestLogWidget->SetPositionInViewport(FVector2D((viewportData.ViewportSize.X * inventoryPosition.ViewportRatio.X) + ((questLogPosition.ViewportOffset.X - inventoryPosition.ViewportOffset.X) * viewportData.ViewportScale), (viewportData.ViewportSize.Y * inventoryPosition.ViewportRatio.Y) + ((questLogPosition.ViewportOffset.Y - inventoryPosition.ViewportOffset.Y)*viewportData.ViewportScale)));
			}
			else {
				m_Widgets.QuestLogWidget->SetPositionInViewport(FVector2D((viewportData.ViewportSize.X * questLogPosition.ViewportRatio.X) + (questLogPosition.ViewportOffset.X * viewportData.ViewportScale), (viewportData.ViewportSize.Y * questLogPosition.ViewportRatio.Y) + (questLogPosition.ViewportOffset.Y*viewportData.ViewportScale)));
			}
		}
		m_Widgets.QuestLogWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

AActor* ARPGDemoCharacter::GetClickedActor()
{
	FHitResult theHit;
	if (!Controller) return NULL;
	Controller->CastToPlayerController()->GetHitResultUnderCursor(ECC_Pawn, false, theHit);
	if (theHit.bBlockingHit)
	{
		if (theHit.Actor == NULL)
			return NULL;

		if (theHit.GetActor()->ActorHasTag(FName("Targetable")))
			return theHit.GetActor();
		else if (theHit.GetActor()->ActorHasTag(FName("ItemPickup")))
		{
			AItemPickupActorParent* theItem = Cast<AItemPickupActorParent>(theHit.GetActor());
			if (!theItem) return NULL;
			ServerPickupItem(theHit.GetActor());
		}
		else if (theHit.GetActor()->ActorHasTag(FName("Lootable")))
		{
			ALootableActorParent* theChest = Cast<ALootableActorParent>(theHit.GetActor());
			if (!theChest) return NULL;
			ServerToggleChest(theHit.GetActor());
		}
	}
	return NULL;
}

bool ARPGDemoCharacter::HasShieldEquipped()
{
	AWornEquipmentActorParent* theItem = UStaticFunctions::GetEquipmentSlot(this, EEquipmentSlot::LeftHand);
	if (theItem)
	{
		FString itemName = theItem->m_ItemName;
		if (itemName.ToLower().Contains("shield"))
			return true;
		else
			return false;
	}
	else
		return false;
}

void ARPGDemoCharacter::InflictDamage(int32 amount, AActor* inflictor)
{
	int32 attemptBlockChance = FMath::RandRange(1, 100);
	int32 blockChance = FMath::RandRange(1, 4);
	int32 modifiedDamage = FMath::Clamp((int32)(amount - (m_EquipmentStats.DefensePoints * 0.1)), (int32)(amount*0.2), amount);
	FVector2D posOffset = FVector2D(FMath::FRandRange(0.3, 0.6), FMath::FRandRange(0.3, 0.6));
	FVector2D theOffset = FVector2D(FMath::FRandRange(-50, 50), FMath::FRandRange(-100, -50));
	if (HasShieldEquipped() && ( (attemptBlockChance < m_EquipmentStats.DefensePoints && m_EquipmentStats.DefensePoints < 60) || (attemptBlockChance < m_EquipmentStats.DefensePoints && m_EquipmentStats.DefensePoints >= 60 && blockChance > 3) ) && m_Health > 0)
	{
		if (!IsAbilityMontagePlaying())
		{
			ClientStopAllAnimMontages();
			ClientPlayAnimMontage(m_AnimMontages.Block);
		}
		ClientAddFloatingTextEx("Blocked!", FLinearColor(1, 1, 1, 1), "/Game/Fonts/RifficRegular", FVector2D(3, 3), posOffset, theOffset, 2.5f, true, true, 1, false, FLinearColor(0, 0, 0, 0), true, 3);
		ClientPlaySoundAtLocation(m_ShieldBlockSound, GetActorLocation());
	}
	else if (m_Health > 0)
	{

		if(!GetMesh()->AnimScriptInstance->Montage_IsPlaying(m_AnimMontages.Attack) && !GetMesh()->AnimScriptInstance->Montage_IsPlaying(m_AnimMontages.AttackTwo) && !IsAbilityMontagePlaying() )
			ClientPlayAnimMontage(m_AnimMontages.Hurt);
		m_Health = FMath::Clamp(m_Health - modifiedDamage, 0, m_MaxHealth);
		if (m_Health == 0)
		{
			if (!m_IsDead)
			{
				m_IsDead = true;
				ClientStopAllAnimMontages();
				ClientPlayAnimMontage(m_AnimMontages.Death);
				GetWorld()->GetTimerManager().SetTimer(m_RespawnTimerHandle, this, &ARPGDemoCharacter::RespawnPlayer, 3, false);
				for (TActorIterator<ABotCharacterParent> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					if (ActorItr->m_AggroMap.Contains(this))
						ActorItr->m_AggroMap.Remove(this);
				}
				//return;
			}
		}
		ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(inflictor);
		if (thePlayer)
			thePlayer->ClientAddFloatingTextEx("-" + FString::FromInt(modifiedDamage), FLinearColor(0.5, 0, 0, 1), "/Game/Fonts/Homoarakhn", FVector2D(2, 2), posOffset, theOffset, 2.5f, false, false, 0, true, FLinearColor(1, 0, 0, 1), true, 3);
	}
}

void ARPGDemoCharacter::RespawnPlayer()
{
	m_Health = m_MaxHealth;
	m_Energy = m_MaxEnergy;
	m_IsDead = false;
	m_IsStunned = false;
	ResetAbilityCooldowns();
	TArray<FVector> spawnPointArray;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if(ActorItr->ActorHasTag("SpawnPoint"))
			spawnPointArray.Add(ActorItr->GetActorLocation());
	}
	
	SetActorLocation(spawnPointArray[FMath::RandRange(0, spawnPointArray.Num()-1)]);
}

void ARPGDemoCharacter::TakeEnergy(int32 amount)
{
	m_Energy = FMath::Clamp(m_Energy - amount, 0, m_MaxEnergy);
}

void ARPGDemoCharacter::OnCombatTimerTick()
{
	if (m_Target && !m_IsDead && !m_IsStunned)
	{
		ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(m_Target);
		ABotCharacterParent* theBot = Cast<ABotCharacterParent>(m_Target);
		if ((thePlayer && thePlayer->m_IsDead) || (theBot && theBot->m_Health == 0))
		{
			GetWorld()->GetTimerManager().ClearTimer(m_CombatHandle);
			ServerSetCombatStatus(false);
			ServerSetTarget(NULL);
			return;
		}
		if (FVector::Dist(GetActorLocation(), m_Target->GetActorLocation()) < m_MeleeAttackRange)
		{
			if (UStaticFunctions::ActorIsFacingLocation(this, m_Target->GetActorLocation()))
			{
				AWornEquipmentActorParent* theItem = UStaticFunctions::GetEquipmentSlot(this, EEquipmentSlot::RightHand);
				FWeaponStats theStats = UStaticFunctions::GetWeaponStats(theItem);
				int32 damage = FMath::Clamp(theStats.AttackPoints + FMath::RandRange(-(theStats.AttackPoints*0.2), theStats.AttackPoints*0.2), 0, (int32)(theStats.AttackPoints + theStats.AttackPoints*0.2));
				ServerInflictDamage(damage, m_Target);
				TArray<FParticleParameter> parameterArray;
				FParticleParameter scaleParameter = FParticleParameter();
				scaleParameter.ParameterName = "ParticleScale";
				scaleParameter.ParameterType = EParticleParameterTypes::Vector;
				scaleParameter.VectorValue = FVector(30, 30, 30);
				parameterArray.Add(scaleParameter);
				FVector posOffset = FVector(FMath::FRandRange(-30, 30), FMath::FRandRange(-30, 30), FMath::FRandRange(-30, 30));
				ClientSpawnParticleEmitterAtLocation("/Game/ParticleSystems/PS_BloodSplatter", m_Target->GetActorLocation() + posOffset, FRotator(0, 0, 0), parameterArray);
				if (m_Energy >= 10 && !IsAbilityMontagePlaying()) {
					StopAllAnimMontages();
					UAnimMontage* attackMontage = m_AttackMontages[FMath::RandRange(0, m_AttackMontages.Num() - 1)];
					PlayAnimMontage(attackMontage);
					if(UStaticFunctions::GetEquipmentSlot(this, EEquipmentSlot::RightHand))
						ClientPlaySoundAtLocation(m_SwordSwingSound, GetActorLocation());
				}

			}
			else
				ClientDisplayErrorMessage("Not Facing Target");
		}
		else
			ClientDisplayErrorMessage("Out of Range");
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(m_CombatHandle);
		ServerSetCombatStatus(false);
	}
}

bool ARPGDemoCharacter::IsEquipmentSlotEmpty(EEquipmentSlot equipSlot)
{
	return (UStaticFunctions::GetEquipmentSlot(this, equipSlot) == NULL);
}

void ARPGDemoCharacter::EquipSlot(AWornEquipmentActorParent* equipActor)
{
	if (m_IsStunned)
	{
		ClientDisplayErrorMessage("You're Currently Stunned");
		return;
	}
	int32 itemID = UStaticFunctions::GetItemIDByName(equipActor->m_ItemName);
	FVector2D posOffset = FVector2D(FMath::FRandRange(0.3, 0.6), FMath::FRandRange(0.3, 0.6));
	FVector2D theOffset = FVector2D(FMath::FRandRange(-50, 50), FMath::FRandRange(-100, -50));
	ClientDisplayNotifyMessage("You Equipped a " + equipActor->m_ItemName + "!");
	EEquipmentSlot equipSlot = UStaticFunctions::GetEquipmentSlotByID(itemID);
	UStaticFunctions::SetEquipmentSlot(this, equipSlot, equipActor);
	m_EquipmentStats = UStaticFunctions::GetEquipmentStats(this);
	int32 healthPoints = m_EquipmentStats.HealthPoints;
	if (m_Health == m_MaxHealth)
	{
		m_MaxHealth = 100 + healthPoints;
		m_Health = m_MaxHealth;
	}
	else
		m_MaxHealth = 100 + healthPoints;
}

void ARPGDemoCharacter::UnEquipSlot(EEquipmentSlot equipSlot)
{
	if (!IsEquipmentSlotEmpty(equipSlot))
	{
		if (m_Inventory.Num() >= m_MaxInventoryItems)
		{
			ClientDisplayErrorMessage("Inventory is Full");
			return;
		}
		if (m_IsStunned)
		{
			ClientDisplayErrorMessage("You're Currently Stunned");
			return;
		}
		AWornEquipmentActorParent* theActor = UStaticFunctions::GetEquipmentSlot(this, equipSlot);
		int32 itemID = UStaticFunctions::GetItemIDByName(theActor->m_ItemName);
		m_Inventory.Add(itemID);
		UpdateQuestProgress(NULL, UStaticFunctions::m_ItemMap[itemID].ItemName, 1);
		m_EquipmentStats = UStaticFunctions::GetEquipmentStats(this);
		//ClientAddInventoryItem(itemID);
		UStaticFunctions::SetEquipmentSlot(this, equipSlot, NULL);
		if (theActor){
			theActor->GetRootComponent()->DetachFromParent();
			theActor->Destroy();
		}
	}
}

void ARPGDemoCharacter::AttemptEnergyRegen()
{
	if (m_Energy == m_MaxEnergy) return;
	if (m_IsDead) return;
	int32 regenBonus = m_EquipmentStats.EnergyPoints;
	m_Energy = FMath::Clamp(m_Energy + m_EnergyRegenBaseAmount + regenBonus, 0, m_MaxEnergy);
}

FString ARPGDemoCharacter::GetPersonaName()
{
	if (SteamAPI_Init())
	{
		const char* personaName = SteamFriends()->GetPersonaName();
		return personaName;
	}
	else {
		if (m_DisplayName == "")
			return UStaticFunctions::GetRandomName();
		else
			return m_DisplayName;
	}
	return "NULL";
}

uint64 ARPGDemoCharacter::GetSteamID64()
{
	if (SteamAPI_Init())
	{
		CSteamID steamID = SteamUser()->GetSteamID();
		uint64 steam64 = steamID.ConvertToUint64();
		
		return steam64;
	}
	return 0;
}

FString ARPGDemoCharacter::GetLegacySteamID()
{
	if (SteamAPI_Init())
	{
		CSteamID steamID = SteamUser()->GetSteamID();
		return UStaticFunctions::SteamID64ToLegacy(steamID.ConvertToUint64());
	}
	return FString("NULL");
}

bool ARPGDemoCharacter::IsAbilityMontagePlaying()
{
	if (GetMesh()->AnimScriptInstance->Montage_IsPlaying(m_AnimMontages.Pushback)) return true;
	if (GetMesh()->AnimScriptInstance->Montage_IsPlaying(m_AnimMontages.Knockout)) return true;
	if (GetMesh()->AnimScriptInstance->Montage_IsPlaying(m_AnimMontages.DrainLife)) return true;
	if (GetMesh()->AnimScriptInstance->Montage_IsPlaying(m_AnimMontages.HeavyAttack)) return true;
	if (GetMesh()->AnimScriptInstance->Montage_IsPlaying(m_AnimMontages.Strikedown)) return true;
	return false;
}

void ARPGDemoCharacter::StartAnimMontage(UAnimMontage* animMontage, float playRate, FName startSectionName)
{
	USkeletalMeshComponent* useMesh = GetMesh();
	if (animMontage && useMesh && useMesh->AnimScriptInstance)
	{
		if (useMesh->AnimScriptInstance->Montage_IsPlaying(animMontage))
			return;
		useMesh->AnimScriptInstance->Montage_Play(animMontage, playRate);
	}
}

bool ARPGDemoCharacter::IsAbilityOnCooldown(EPlayerAbilities abilityEnum)
{
	if (!m_AbilityCooldowns.Contains(abilityEnum) || !GetWorld())
		return false;
	float currentTime = GetWorld()->GetRealTimeSeconds();
	float cooldownEndTime = (float)m_AbilityCooldowns[abilityEnum];
	if (currentTime > cooldownEndTime)
	{
		m_AbilityCooldowns.Remove(abilityEnum);
		return false;
	}
	else
		return true;
}

void ARPGDemoCharacter::TriggerAbilityCooldown(EPlayerAbilities abilityEnum)
{
	if (!GetWorld()) return;
	if (!UStaticFunctions::m_AbilityMap.Contains(abilityEnum)) return;
	float currentTime = GetWorld()->GetRealTimeSeconds();
	FAbilityData abilityStruct = (FAbilityData)UStaticFunctions::m_AbilityMap[abilityEnum];
	m_AbilityCooldowns.Add(abilityEnum, currentTime + abilityStruct.Cooldown);
	ClientTriggerAbilityCooldown(abilityEnum);
}

void ARPGDemoCharacter::ResetAbilityCooldowns()
{
	m_AbilityCooldowns.Empty();
	ClientResetAbilityCooldowns();
}

bool ARPGDemoCharacter::IsFacingTarget()
{
	if (!m_Target) return false;
	if (UStaticFunctions::ActorIsFacingLocation(this, m_Target->GetActorLocation()))
		return true;
	else
		return false;
}

bool ARPGDemoCharacter::IsInAttackRange()
{
	if (!m_Target) return false;
	if (FVector::Dist(GetActorLocation(), m_Target->GetActorLocation()) < m_MeleeAttackRange)
		return true;
	else
		return false;
}

void ARPGDemoCharacter::StopAllAnimMontages()
{
	USkeletalMeshComponent* useMesh = GetMesh();
	if (useMesh && useMesh->AnimScriptInstance)
		useMesh->AnimScriptInstance->Montage_Stop(0.0f);
}

int32 ARPGDemoCharacter::RequiredXPToNextLevel(int32 level)
{
	int32 modifier = level * 50;
	return ((modifier + ((level + (level / 2))*(level / 2) + (level / 5)) * 2 + 3) / 10) * 10;
}

void ARPGDemoCharacter::GiveXP(int32 xpAmount)
{
	m_Experience += xpAmount;
	FVector2D posOffset = FVector2D(FMath::FRandRange(0.3, 0.6), FMath::FRandRange(0.3, 0.6));
	FVector2D theOffset = FVector2D(FMath::FRandRange(-50, 50), FMath::FRandRange(-100, -50));
	ClientAddFloatingTextEx( "+" + FString::FromInt(xpAmount) + "XP", FLinearColor(1, 1, 1, 1), "/Game/Fonts/RifficRegular", FVector2D(2, 2), posOffset, theOffset, 2.5f, true, true, 1, false, FLinearColor(0, 0, 0, 0), true, 3);
	if (m_Experience > RequiredXPToNextLevel(m_Level+1))
	{
		m_Level += 1;
		SetHealthBoost();
		ClientDisplayNotifyMessage("You Reached Level " + FString::FromInt(m_Level) + "!");
	}
}

void ARPGDemoCharacter::SetHealthBoost()
{
	m_MaxHealth = 100 + m_HealthLevelMultiplier * m_Level;
	m_Health = m_MaxHealth;
}

int32 ARPGDemoCharacter::GetDamageBoost(int32 damage)
{
	int32 damageMod = damage * (FMath::Clamp(m_Level, 0, m_Level) / 5);
	int32 damageBoost = FMath::RandRange(damageMod * 0.5, damageMod * 1.5);
	return damage + damageBoost;
}

void ARPGDemoCharacter::TriggerStun(float length)
{
	if (m_IsStunned) return;
	m_IsStunned = true;
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=] {
		if (this)
			m_IsStunned = false;
	});
	UStaticFunctions::TriggerDelayedAction(GetWorld(), timerDelegate, length);
}

bool ARPGDemoCharacter::HasQuest(FString questName)
{
	for (FQuestDetails questDetails : m_Quests)
	{
		if (questDetails.Name == questName)
			return true;
	}
	return false;
}

bool ARPGDemoCharacter::IsQuestComplete(FString questName)
{
	for (FQuestDetails questDetails : m_Quests)
	{
		if (questDetails.Name == questName)
		{
			return (questDetails.Completed);
		}
	}
	return false;
}


void ARPGDemoCharacter::GiveQuest(FString questName)
{
	if (HasQuest(questName)) return;
	if (!UStaticFunctions::m_QuestMap.Contains(questName)) return;
	FQuestDetails questTemplate = (FQuestDetails)UStaticFunctions::m_QuestMap[questName];
	FQuestDetails questStruct = FQuestDetails();
	questStruct.Name = questTemplate.Name;
	questStruct.Description = questTemplate.Description;
	questStruct.Type = questTemplate.Type;
	questStruct.ItemRetrievalProgress = FItemRetrievalQuestDetails();
	questStruct.ItemRetrievalProgress.RequiredItems = questTemplate.ItemRetrievalProgress.RequiredItems;
	questStruct.ItemRetrievalProgress.ItemName = questTemplate.ItemRetrievalProgress.ItemName;
	questStruct.SlayingProgress = FSlayingQuestDetails();
	questStruct.SlayingProgress.RequiredKills = questTemplate.SlayingProgress.RequiredKills;
	questStruct.SlayingProgress.FactionName = questTemplate.SlayingProgress.FactionName;
	m_Quests.Add(questStruct);
	ClientGiveQuest(questName);
}

void ARPGDemoCharacter::RemoveQuest(FString questName)
{
	int32 questIndex = 0;
	bool foundQuest;
	if (HasQuest(questName))
	{
		for (int i = 0; i < m_Quests.Num(); i++)
		{
			if (m_Quests[i].Name == questName)
			{
				int32 questIndex = i;
				foundQuest = true;
				break;
			}
		}
		if (foundQuest)
			m_Quests.RemoveAt(questIndex);
	}
	ClientRemoveQuest(questName);
}

void ARPGDemoCharacter::UpdateQuestProgress(AActor* theActor, FString itemName, int32 addVal)
{
	TArray<int32> questIndexes;
	EQuestTypes questType;
	FString factionName = "";
	bool questCompleted = false;

	if (theActor)
	{
		ABotCharacterParent* theBot = Cast<ABotCharacterParent>(theActor);
		if (theBot)
			factionName = theBot->m_Faction;
		questType = EQuestTypes::Slaying;
	}
	else
		questType = EQuestTypes::ItemRetrieval;

	for (int i = 0; i < m_Quests.Num(); i++)
	{
		FQuestDetails questDetails = m_Quests[i];
		if (questDetails.Completed) continue;
		switch (questType)
		{
			case EQuestTypes::Slaying:
			{
				bool isFaction = (questDetails.SlayingProgress.FactionName == factionName);
				bool isSlayingQuest = (questDetails.Type == EQuestTypes::Slaying);
				if (isSlayingQuest && isFaction)
					questIndexes.Add(i);
			}
			break;
			case EQuestTypes::ItemRetrieval:
			{
				bool isItem = (questDetails.ItemRetrievalProgress.ItemName == itemName);
				bool isRetrievalQuest = (questDetails.Type == EQuestTypes::ItemRetrieval);
				if (isRetrievalQuest && isItem)
					questIndexes.Add(i);
			}
			break;
		}
	}
	for (int i = 0; i < questIndexes.Num(); i++)
	{
		switch (questType)
		{
			case EQuestTypes::Slaying:
			{
				m_Quests[questIndexes[i]].SlayingProgress.CurrentKills = m_Quests[questIndexes[i]].SlayingProgress.CurrentKills + addVal;
				ClientUpdateQuestProgress(m_Quests[questIndexes[i]].Name, m_Quests[questIndexes[i]].SlayingProgress.CurrentKills);
				if (m_Quests[questIndexes[i]].SlayingProgress.CurrentKills == m_Quests[questIndexes[i]].SlayingProgress.RequiredKills)
					questCompleted = true;
			}
			break;
			case EQuestTypes::ItemRetrieval:
			{
				m_Quests[questIndexes[i]].ItemRetrievalProgress.CurrentItems = m_Quests[questIndexes[i]].ItemRetrievalProgress.CurrentItems + addVal;
				ClientUpdateQuestProgress(m_Quests[questIndexes[i]].Name, m_Quests[questIndexes[i]].ItemRetrievalProgress.CurrentItems);
				if (m_Quests[questIndexes[i]].ItemRetrievalProgress.CurrentItems == m_Quests[questIndexes[i]].ItemRetrievalProgress.RequiredItems)
					questCompleted = true;
			}
			break;
		}
		if (questCompleted)
		{
			m_Quests[questIndexes[i]].Completed = true;
			ClientDisplayNotifyMessage("You Completed the Quest " + m_Quests[questIndexes[i]].Name + "!");
			GiveXP(m_Quests[questIndexes[i]].XPReward);
			for (FString itemName : m_Quests[questIndexes[i]].ItemRewards)
			{
				int32 itemID = UStaticFunctions::GetItemIDByName(itemName);
				if (!UStaticFunctions::ItemIDExists(itemID)) continue;
				if (m_Inventory.Num() < m_MaxInventoryItems)
					m_Inventory.Add(itemID);
			}
		}
	}
}

void ARPGDemoCharacter::ServerSetTarget_Implementation(AActor* theTarget)
{
	if (theTarget == this) return;
	m_Target = theTarget;
}

bool ARPGDemoCharacter::ServerSetTarget_Validate(AActor* theTarget)
{
	return true;
}

void ARPGDemoCharacter::ServerPlayerUseAction_Implementation(int32 index)
{
	EPlayerAbilities abilityEnum = UStaticFunctions::IndexToActionSlot(index, this);
	if (m_IsStunned)
	{
		ClientDisplayErrorMessage("You're Currently Stunned");
		return;
	}
	if (!IsFacingTarget())
	{
		ClientDisplayErrorMessage("Not Facing Target");
		return;
	}
	if (!IsInAttackRange())
	{
		ClientDisplayErrorMessage("Out of Range");
		return;
	}
	if (IsAbilityOnCooldown(abilityEnum))
	{
		ClientDisplayErrorMessage("Ability is on Cooldown");
		return;
	}
	switch (abilityEnum)
	{
		case EPlayerAbilities::HeavyAttack:
			UStaticPlayerAbilities::AttemptHeavyAttack(this);
		break;
		case EPlayerAbilities::Knockout:
			UStaticPlayerAbilities::AttemptKnockout(this);
		break;
		case EPlayerAbilities::WeakPushback:
			UStaticPlayerAbilities::AttemptWeakPushback(this);
		break;
		case EPlayerAbilities::WeakLifeDrain:
			UStaticPlayerAbilities::AttemptWeakLifeDrain(this);
		break;
		case EPlayerAbilities::Strikedown:
			UStaticPlayerAbilities::AttemptStrikedown(this);
		break;
	}
	TriggerAbilityCooldown(abilityEnum);
}

bool ARPGDemoCharacter::ServerPlayerUseAction_Validate(int32 index)
{
	return true;
}

void ARPGDemoCharacter::ServerSendSteamData_Implementation(const FString& steam64, const FString& legacyID, const FString& personaName, const FString& ipAddress)
{
	//Shitty security measure against forging data.
	if (m_Steam64 != "" && m_LegacySteamID != "")
		return;
	m_Steam64 = steam64;
	m_LegacySteamID = legacyID;
	m_DisplayName = personaName;
	if (m_Steam64 == "0")
	{
		m_Steam64 = ipAddress;
		m_LegacySteamID = ipAddress;
	}
	FPlayerData playerData = FPlayerData();
	TArray<int32> equipmentArray;
	UStaticFunctions::LoadGameDataFromPath("E:/TestData/" + m_Steam64 + ".dat", playerData, m_Inventory, equipmentArray);
	if (playerData.DisplayName != "")
		m_DisplayName = playerData.DisplayName;
	m_Level = playerData.Level;
	m_Experience = playerData.Experience;
	SetHealthBoost();
	UStaticFunctions::LoadEquipmentArray(equipmentArray, this);
}

bool ARPGDemoCharacter::ServerSendSteamData_Validate(const FString& steam64, const FString& legacyID, const FString& personaName, const FString& ipAddress)
{
	return true;
}

void ARPGDemoCharacter::ServerInflictDamage_Implementation(int32 amount, AActor* theTarget)
{
	if (!theTarget) return;
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theTarget);
	ABotCharacterParent* theBot = Cast<ABotCharacterParent>(theTarget);
	if (!thePlayer && !theBot) return;
	if (m_Energy < 10)
	{
		ClientDisplayErrorMessage("You don't have enough energy.");
		return;
	}
	else
	{
		if (thePlayer)
			thePlayer->InflictDamage(amount, this);
		else if (theBot)
			theBot->InflictDamage(amount, this);
		m_Energy = FMath::Clamp(m_Energy - 10, 0, m_MaxEnergy);
	}
}

bool ARPGDemoCharacter::ServerInflictDamage_Validate(int32 amount, AActor* theTarget)
{
	return true;
}

void ARPGDemoCharacter::ServerSetCombatStatus_Implementation(bool aBool)
{
	m_InCombat = aBool;
}

bool ARPGDemoCharacter::ServerSetCombatStatus_Validate(bool aBool)
{
	return true;
}

void ARPGDemoCharacter::ServerToggleChest_Implementation(AActor* theActor)
{
	ALootableActorParent* theObject= Cast<ALootableActorParent>(theActor);
	if (!theObject) return;

	if (m_IsStunned)
	{
		ClientDisplayErrorMessage("You're Currently Stunned");
		return;
	}

	if (theObject->m_CurrentLooter && theObject->m_CurrentLooter != this)
	{
		ClientDisplayErrorMessage("That Object is Busy");
		return;
	}

	if (FVector::Dist(theActor->GetActorLocation(), GetActorLocation()) > 250)
	{
		ClientDisplayErrorMessage("Out of Range");
		return;
	}
	theObject->ToggleStatus(this);
}

bool ARPGDemoCharacter::ServerToggleChest_Validate(AActor* theActor)
{
	return true;
}

void ARPGDemoCharacter::ServerAttemptLootChest_Implementation(AActor* theActor, int32 itemID)
{
	ALootableActorParent* theObject = Cast<ALootableActorParent>(theActor);
	if (!theObject) return;

	if (this != theObject->m_CurrentLooter) return;

	if (m_IsStunned)
	{
		ClientDisplayErrorMessage("You're Currently Stunned");
		return;
	}

	if (m_Inventory.Num() >= m_MaxInventoryItems)
	{
		ClientDisplayErrorMessage("Inventory is Full");
		return;
	}

	if (theObject->m_Loot.Contains(itemID)) 
	{
		theObject->m_Loot.RemoveSingle(itemID);
		m_Inventory.Add(itemID);
		UpdateQuestProgress(NULL, UStaticFunctions::m_ItemMap[itemID].ItemName, 1);
		if (theObject->m_Loot.Num() == 0) {
			theObject->m_HasLoot = false;
			theObject->m_Opened = false;
			GetWorld()->GetTimerManager().ClearTimer(theObject->m_AutoCloseHandle);
			GetWorld()->GetTimerManager().SetTimer(theObject->m_DestroyHandle, theObject, &ALootableActorParent::TriggerDestruction, 5, false);
			ClientCloseLootPanel();
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(theObject->m_AutoCloseHandle, theObject, &ALootableActorParent::AutoClose, 5, false);
		}
	}
}

bool ARPGDemoCharacter::ServerAttemptLootChest_Validate(AActor* theActor, int32 itemID)
{
	return true;
}

void ARPGDemoCharacter::ServerPickupItem_Implementation(AActor* theActor)
{
	AItemPickupActorParent* theItem = Cast<AItemPickupActorParent>(theActor);
	if (!theItem) return;
	if (m_IsStunned)
	{
		ClientDisplayErrorMessage("You're Currently Stunned");
		return;
	}
	if (FVector::Dist(theActor->GetActorLocation(), GetActorLocation()) > 250)
	{
		ClientDisplayErrorMessage("Out of Range");
		return;
	}
	ClientPlayAnimMontage(m_AnimMontages.Loot);
	ServerAddInventoryItem(UStaticFunctions::GetItemIDByName(theItem->m_ItemName));
	theItem->Destroy();
}

bool ARPGDemoCharacter::ServerPickupItem_Validate(AActor* theActor)
{
	return true;
}

void ARPGDemoCharacter::ServerAddInventoryItem_Implementation(int32 itemID)
{
	int totalItems = m_Inventory.Num();
	if (totalItems >= m_MaxInventoryItems) {
		ClientDisplayErrorMessage("Inventory is Full");
		return;
	}
	else {
		m_Inventory.Add(itemID);
		UpdateQuestProgress(NULL, UStaticFunctions::m_ItemMap[itemID].ItemName, 1);
	}
}

bool ARPGDemoCharacter::ServerAddInventoryItem_Validate(int32 itemID)
{
	return true;
}

void ARPGDemoCharacter::ServerRemoveInventoryItem_Implementation(int32 itemID)
{
	if (m_Inventory.Contains(itemID)) {
		m_Inventory.RemoveSingle(itemID);
		UpdateQuestProgress(NULL, UStaticFunctions::m_ItemMap[itemID].ItemName, -1);
	}
}

bool ARPGDemoCharacter::ServerRemoveInventoryItem_Validate(int32 itemID)
{
	return true;
}

void ARPGDemoCharacter::OnRep_InventoryChanged()
{
	if (m_Widgets.InventoryWidget)
	{
		UInventoryWidget* theWidget = Cast<UInventoryWidget>(m_Widgets.InventoryWidget);
		theWidget->GenerateInventoryIcons();
	}
}

void ARPGDemoCharacter::ServerConsumeItem_Implementation(int32 itemID)
{
	if (!UStaticFunctions::ItemIDExists(itemID)) return;
	if (!m_Inventory.Contains(itemID)) return;
	if (m_IsStunned)
	{
		ClientDisplayErrorMessage("You're Currently Stunned");
		return;
	}
	FConsumableData consumeStruct = UStaticFunctions::m_ConsumableDataMap[itemID];
	switch (consumeStruct.ConsumeType)
	{
		case EConsumeType::Health:
			m_Health = FMath::Clamp(m_Health + consumeStruct.PointAmount, 0, m_MaxHealth);
		break;
		case EConsumeType::Energy:
			m_Energy = FMath::Clamp(m_Energy + consumeStruct.PointAmount, 0, m_MaxEnergy);
		break;
	}
	ClientDisplayNotifyMessage("You've Consumed a " + UStaticFunctions::m_ItemMap[itemID].ItemName + "!");
	m_Inventory.RemoveSingle(itemID);
	UpdateQuestProgress(NULL, UStaticFunctions::m_ItemMap[itemID].ItemName, -1);
}

bool ARPGDemoCharacter::ServerConsumeItem_Validate(int32 itemID)
{
	return true;
}

void ARPGDemoCharacter::ServerEquipSlotByID_Implementation(int32 itemID)
{
	if (!GetWorld()) return;
	if (!UStaticFunctions::ItemIDExists(itemID)) return;
	if (m_IsStunned)
	{
		ClientDisplayErrorMessage("You're Currently Stunned");
		return;
	}
	EEquipmentSlot equipSlot = UStaticFunctions::GetEquipmentSlotByID(itemID);
	if (!UStaticFunctions::GetEquipmentSlot(this, equipSlot) == NULL)
	{
		ClientDisplayErrorMessage("That Slot Isn't Empty");
		return;
	}
	if (!m_Inventory.Contains(itemID)) return;
	m_Inventory.RemoveSingle(itemID);
	UpdateQuestProgress(NULL, UStaticFunctions::m_ItemMap[itemID].ItemName, -1);
	AttachEquipment(itemID);
}

bool ARPGDemoCharacter::ServerEquipSlotByID_Validate(int32 itemID)
{
	return true;
}

void ARPGDemoCharacter::ServerUnEquipSlot_Implementation(EEquipmentSlot equipSlot)
{
	if (UStaticFunctions::GetEquipmentSlot(this, equipSlot) == NULL) return;
	if (m_Inventory.Num() >= m_MaxInventoryItems) return;
	UnEquipSlot(equipSlot);
}

bool ARPGDemoCharacter::ServerUnEquipSlot_Validate(EEquipmentSlot equipSlot)
{
	return true;
}

void ARPGDemoCharacter::ServerLoadGameData_Implementation(const FString& steam64)
{
	if (steam64 == "0")
	{
		UStaticFunctions::DebugMessage(FColor::Red, "Couldn't get player's Steam64");
	}
	else
	{
		FPlayerData playerData = FPlayerData();
		TArray<int32> equipmentArray;
		UStaticFunctions::LoadGameDataFromPath("E:/TestData/" + steam64 + ".dat", playerData, m_Inventory, equipmentArray);
		m_DisplayName = playerData.DisplayName;
		m_Level = playerData.Level;
		m_Experience = playerData.Experience;
		m_MaxHealth = 100 + m_HealthLevelMultiplier * m_Level;
		m_Health = m_MaxHealth;
		SetHealthBoost();
		UStaticFunctions::LoadEquipmentArray(equipmentArray, this);
	}
}

bool ARPGDemoCharacter::ServerLoadGameData_Validate(const FString& steam64)
{
	return true;
}

void ARPGDemoCharacter::ServerSaveGameData_Implementation(const FString& steam64, const FString& personaName)
{
	FPlayerData playerData = FPlayerData();
	playerData.DisplayName = personaName;
	playerData.Rank = "user";
	playerData.Level = m_Level;
	playerData.Experience = m_Experience;
	m_DisplayName = personaName;
	TArray<int32> equipmentArray = UStaticFunctions::GetEquipmentArray(m_Equipment);
	FString _steam64 = steam64;
	if (_steam64 == "0")
		_steam64 = m_Steam64;

	if (_steam64 == "0")
	{
		UStaticFunctions::DebugMessage(FColor::Red, "Couldn't get player's Steam64");
	}
	else
	{
		FBufferArchive toBinary;
		UStaticFunctions::SaveGameDataToPath("E:/TestData/" + _steam64 + ".dat", toBinary, playerData, m_Inventory, equipmentArray);
	}
}

bool ARPGDemoCharacter::ServerSaveGameData_Validate(const FString& steam64, const FString& personaName)
{
	return true;
}

void ARPGDemoCharacter::ServerSendChatMessage_Implementation(const FString& theText)
{
	ParseServerCommand(theText);
	for (TActorIterator<ARPGDemoCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->ClientRelayChatMessage(m_DisplayName + ": " + theText);
	}
}

bool ARPGDemoCharacter::ServerSendChatMessage_Validate(const FString& theText)
{
	return true;
}

void ARPGDemoCharacter::ClientGiveQuest_Implementation(const FString& questName)
{
	GiveQuest(questName);
	UQuestLogPanelWidget* logPanel = Cast<UQuestLogPanelWidget>(m_Widgets.QuestLogWidget);
	if (logPanel)
		logPanel->GenerateQuestSlots();
}

void ARPGDemoCharacter::ClientRemoveQuest_Implementation(const FString& questName)
{
	RemoveQuest(questName);
	UQuestLogPanelWidget* logPanel = Cast<UQuestLogPanelWidget>(m_Widgets.QuestLogWidget);
	if (logPanel)
		logPanel->GenerateQuestSlots();
}

void ARPGDemoCharacter::ClientUpdateQuestProgress_Implementation(const FString& questName, int32 newVal)
{
	int32 questIndex = -1;
	for (int i = 0; i < m_Quests.Num(); i++)
	{
		if (m_Quests[i].Name == questName)
		{
			questIndex = i;
			break;
		}
	}
	if (questIndex != -1)
	{
		switch (m_Quests[questIndex].Type)
		{
			case EQuestTypes::Slaying:
			{
				m_Quests[questIndex].SlayingProgress.CurrentKills = newVal;
				if (newVal == m_Quests[questIndex].SlayingProgress.RequiredKills)
					m_Quests[questIndex].Completed = true;
			}
			break;
			case EQuestTypes::ItemRetrieval:
			{
				m_Quests[questIndex].ItemRetrievalProgress.CurrentItems = newVal;
				if (newVal == m_Quests[questIndex].ItemRetrievalProgress.RequiredItems)
					m_Quests[questIndex].Completed = true;
			}
			break;
		}
		UQuestLogPanelWidget* logPanel = Cast<UQuestLogPanelWidget>(m_Widgets.QuestLogWidget);
		if (logPanel)
			logPanel->GenerateQuestSlots();
		
	}
}

void ARPGDemoCharacter::ClientSpawnParticleEmitterAtLocation_Implementation(const FString& particlePath, FVector location, FRotator rotation, const TArray<FParticleParameter>& parameterArray, float length)
{
	UParticleSystemComponent* particleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), UStaticFunctions::LoadParticleSystemFromPath(*particlePath), location, rotation, true);
	UStaticFunctions::SetParticleEmitterParameters(particleSystem, parameterArray);
	if (length > 0)
		UStaticFunctions::SetObjectForDestruction(GetWorld(), particleSystem, FMath::Clamp(length, 0.0f, FMath::Abs(length)));
}

void ARPGDemoCharacter::ClientSpawnParticleEmitterAttached_Implementation(const FString& particlePath, FName socketName, FVector offset, FRotator rotation, const TArray<FParticleParameter>& parameterArray, float length)
{
	UParticleSystemComponent* particleSystem = UGameplayStatics::SpawnEmitterAttached(UStaticFunctions::LoadParticleSystemFromPath(*particlePath), GetMesh(), socketName, offset, rotation, EAttachLocation::KeepRelativeOffset, true);
	UStaticFunctions::SetParticleEmitterParameters(particleSystem, parameterArray);
	if (length > 0)
		UStaticFunctions::SetObjectForDestruction(GetWorld(), particleSystem, FMath::Clamp(length, 0.0f, FMath::Abs(length)));
}

void ARPGDemoCharacter::ClientDisplayErrorMessage_Implementation(const FString& text)
{
	ACustomPlayerHUD* theHUD = Cast<ACustomPlayerHUD>(Controller->CastToPlayerController()->GetHUD());
	if (theHUD)
	{
		theHUD->AddFloatingText(text, FLinearColor(0.75, 0.22, 0.17, 1), "/Game/Fonts/RifficRegular", FVector2D(2, 2), FVector2D(0.5, 0.3), FVector2D(0, -100), 1.5);
	}
}

void ARPGDemoCharacter::ClientDisplayNotifyMessage_Implementation(const FString& text)
{
	ACustomPlayerHUD* theHUD = Cast<ACustomPlayerHUD>(Controller->CastToPlayerController()->GetHUD());
	if (theHUD)
	{
		theHUD->AddFloatingText(text, FLinearColor(1, 1, 1, 1), "/Game/Fonts/RifficRegular", FVector2D(2, 2), FVector2D(0.5, 0.3), FVector2D(0, -100), 1.5);
	}
}

void ARPGDemoCharacter::ClientAddFloatingText_Implementation(const FString& text, FLinearColor textColor, const FString& fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length)
{
	ACustomPlayerHUD* theHUD = Cast<ACustomPlayerHUD>(Controller->CastToPlayerController()->GetHUD());
	if (theHUD)
	{
		theHUD->AddFloatingText(*text, textColor, *fontPath, textScale, startPos, posOffset, length);
	}
}

void ARPGDemoCharacter::ClientAddFloatingTextEx_Implementation(const FString& text, FLinearColor textColor, const FString& fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length, bool isRainbow, bool sizePulse, float sizePulseStrength, bool colorPulse, FLinearColor otherColor, bool horizontalPulse, float horizontalPulseStrength)
{
	ACustomPlayerHUD* theHUD = Cast<ACustomPlayerHUD>(Controller->CastToPlayerController()->GetHUD());
	if (theHUD)
	{
		theHUD->AddFloatingTextEx(*text, textColor, *fontPath, textScale, startPos, posOffset, length, isRainbow, sizePulse, sizePulseStrength, colorPulse, otherColor, horizontalPulse, horizontalPulseStrength);
	}
}

void ARPGDemoCharacter::ClientStopAllAnimMontages_Implementation()
{
	StopAllAnimMontages();
}

void ARPGDemoCharacter::ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage, float playRate, FName startSectionName)
{
	StartAnimMontage(animMontage, playRate, startSectionName);
}

void ARPGDemoCharacter::ClientOpenLootPanel_Implementation(AActor* theActor)
{
	ALootableActorParent* theObject = Cast<ALootableActorParent>(theActor);
	if (!theObject) return;
	if (m_Widgets.LootWidget) {
		m_Widgets.LootWidget->RemoveFromViewport();
	}
	UCustomUserWidgetParent* lootWidget = CreateWidget<UCustomUserWidgetParent>(Controller->CastToPlayerController(), m_WidgetClasses.LootWidget);
	m_Widgets.LootWidget = lootWidget;
	lootWidget->AddToViewport(0);
	ULootPanelWidget* castedLootWidget = Cast<ULootPanelWidget>(lootWidget);
	if (castedLootWidget)
	{
		theObject->m_CurrentLooter = this;
		castedLootWidget->m_OwningPlayer = this;
		castedLootWidget->m_OwningObject = theActor;
		castedLootWidget->GenerateLootSlots();
	}
}

void ARPGDemoCharacter::ClientCloseLootPanel_Implementation()
{
	if (!m_Widgets.LootWidget) return;
	m_Widgets.LootWidget->RemoveFromViewport();
	m_Widgets.LootWidget = NULL;
}

void ARPGDemoCharacter::ClientRelayChatMessage_Implementation(const FString& theText)
{
	if (m_Widgets.ChatboxWidget)
	{
		UChatboxWidget* theChatbox = Cast<UChatboxWidget>(m_Widgets.ChatboxWidget);
		theChatbox->AddChat(theText);
	}
}

void ARPGDemoCharacter::ClientTriggerAbilityCooldown_Implementation(EPlayerAbilities abilityEnum)
{
	if (!GetWorld()) return;
	if (!UStaticFunctions::m_AbilityMap.Contains(abilityEnum)) return;
	float currentTime = GetWorld()->GetRealTimeSeconds();
	FAbilityData abilityStruct = (FAbilityData)UStaticFunctions::m_AbilityMap[abilityEnum];
	m_AbilityCooldowns.Add(abilityEnum, currentTime + abilityStruct.Cooldown);
}

void ARPGDemoCharacter::ClientResetAbilityCooldowns_Implementation()
{
	m_AbilityCooldowns.Empty();
}

void ARPGDemoCharacter::ClientPlaySoundAtLocation_Implementation(USoundCue* soundCue, FVector location)
{
	UGameplayStatics::PlaySoundAtLocation(this, soundCue, location);
}

void ARPGDemoCharacter::OnRep_EquipmentChanged()
{
	if (m_Widgets.EquipmentWidget)
	{
		UEquipmentWidget* theWidget = Cast<UEquipmentWidget>(m_Widgets.EquipmentWidget);
		theWidget->GenerateEquipmentIcons();
	}
}

void ARPGDemoCharacter::OnRep_BodyColorChanged()
{
	if (!m_BodyMaterialInstance)
	{
		m_BodyMaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
		GetMesh()->SetMaterial(0, m_BodyMaterialInstance);
	}
	m_BodyMaterialInstance->SetVectorParameterValue(TEXT("BodyColor"), m_BodyColor);
}

void ARPGDemoCharacter::OnRep_TargetChanged()
{
	if (!m_Widgets.TargetWidget) return;
	UTargetPanelWidget* targetWidget = Cast<UTargetPanelWidget>(m_Widgets.TargetWidget);
	if (m_Target == NULL)
	{
		targetWidget->ClearTargetActor();
		m_Widgets.TargetWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else 
	{
		m_Widgets.TargetWidget->SetVisibility(ESlateVisibility::Visible);
		targetWidget->SetTargetActor(m_Target);
	}
}