// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"

class UCustomUserWidgetParent;
class AWornEquipmentActorParent;

#include "RPGDemoCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FCommandDelegate, FString);

UCLASS(config=Game)
class ARPGDemoCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Player Variables */
	bool m_FirstPersonToggled;
	float m_LastCameraArmLength;

	FTimerHandle m_CombatHandle;
	FTimerHandle m_RespawnTimerHandle;

	UMaterialInstanceDynamic* m_BodyMaterialInstance;

public:
	ARPGDemoCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UStaticFunctions* staticFuncs = NewObject<UStaticFunctions>(UStaticFunctions::StaticClass());

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Player Variables */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float m_MinScrollAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float m_MaxScrollAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float m_ScrollSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Combat)
	int32 m_CombatTickInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float m_MeleeAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_EnergyRegenBaseAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_EnergyRegenInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_MaxInventoryItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_HealthLevelMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FString m_Username;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TSubclassOf<AActor> m_WornEquipClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	FPlayerActionKeys m_ActionKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	USoundCue* m_SwordSwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	USoundCue* m_ShieldBlockSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	TArray<FQuestDetails> m_Quests;

	FEquipmentStats m_EquipmentStats;

	FTimerHandle m_EnergyRegenHandle;

	FString m_Steam64;

	TMap<EPlayerAbilities, float> m_AbilityCooldowns;

	TMap<FString, FCommandDelegate> m_ServerCommands;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	TArray<UAnimMontage*> m_AttackMontages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	FPlayerAnimMontages m_AnimMontages;

	/** Player UMG Variables */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	FUMGWidgetClasses m_WidgetClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	FPlayerUMGWidgets m_Widgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TArray<FPlayerUMGPosition> m_WidgetPositions;

	/** Player Methods */
	void InflictDamage(int32 amount, AActor* inflictor);
	void TakeEnergy(int32 amount);
	void OnCombatTimerTick();
	bool IsEquipmentSlotEmpty(EEquipmentSlot equipSlot);
	void UnEquipSlot(EEquipmentSlot equipSlot);
	void EquipSlot(AWornEquipmentActorParent* equipActor);
	void ToggleInventoryPanel();
	void ToggleEquipmentPanel();
	void ToggleQuestLogPanel();
	void AttachEquipment(int32 itemID);
	bool HasShieldEquipped();
	void AttemptEnergyRegen();
	void RespawnPlayer();
	void StartAnimMontage(UAnimMontage* animMontage, float playRate = 1.0, FName startSectionName = NAME_None);
	void StopAllAnimMontages();
	void TriggerAbilityCooldown(EPlayerAbilities abilityEnum);
	void ResetAbilityCooldowns();
	bool IsAbilityOnCooldown(EPlayerAbilities abilityEnum);
	bool IsFacingTarget();
	bool IsInAttackRange();
	bool IsAbilityMontagePlaying();
	int32 RequiredXPToNextLevel(int32 level);
	void GiveXP(int32 xpAmount);
	int32 GetDamageBoost(int32 damage);
	void SetHealthBoost();
	void TriggerStun(float length);
	bool HasQuest(FString questName);
	bool IsQuestComplete(FString questName);
	void GiveQuest(FString questName);
	void RemoveQuest(FString questName);
	void UpdateQuestProgress(AActor* theActor = NULL, FString itemName = "", int32 addVal = 1);
	void ParseServerCommand(FString commandText);
	UCustomUserWidgetParent* CreatePlayerWidget(UClass* widgetClass, UCustomUserWidgetParent*& outVar, ESlateVisibility visibility);
	uint64 GetSteamID64();
	FString GetLegacySteamID();
	FString GetPersonaName();

	UFUNCTION()
	void OnRep_EquipmentChanged();
	UFUNCTION()
	void OnRep_InventoryChanged();
	UFUNCTION()
	void OnRep_BodyColorChanged();
	UFUNCTION()
	void OnRep_TargetChanged();

	/** Player Networked Variables */

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_Health;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_MaxHealth;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_Energy;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	FString m_DisplayName;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool m_InCombat;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool m_IsDead;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool m_IsStunned;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Combat)
	FString m_LegacySteamID;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_Level;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 m_Experience;

	UPROPERTY(ReplicatedUsing = OnRep_InventoryChanged, EditAnywhere, BlueprintReadWrite)
	TArray<int32> m_Inventory;

	UPROPERTY(ReplicatedUsing = OnRep_EquipmentChanged, EditAnywhere, BlueprintReadWrite, Category = Character)
	FEquipmentSlots m_Equipment;

	UPROPERTY(ReplicatedUsing = OnRep_TargetChanged, EditAnywhere, BlueprintReadWrite, Category = Character)
	AActor* m_Target;

	UPROPERTY(ReplicatedUsing = OnRep_BodyColorChanged, EditAnywhere, BlueprintReadWrite, Category = Character)
	FLinearColor m_BodyColor;

	/** Player Networked Methods */

	UFUNCTION(NetMulticast, Reliable)
	void ClientSpawnParticleEmitterAtLocation(const FString& particlePath, FVector location, FRotator rotation, const TArray<FParticleParameter>& parameterArray, float length = -1);
	void ClientSpawnParticleEmitterAtLocation_Implementation(const FString& particlePath, FVector location, FRotator rotation, const TArray<FParticleParameter>& parameterArray, float length = -1);

	UFUNCTION(NetMulticast, Reliable)
	void ClientSpawnParticleEmitterAttached(const FString& particlePath, FName socketName, FVector offset, FRotator rotation, const TArray<FParticleParameter>& parameterArray, float length = -1);
	void ClientSpawnParticleEmitterAttached_Implementation(const FString& particlePath, FName socketName, FVector offset, FRotator rotation, const TArray<FParticleParameter>& parameterArray, float length = -1);

	UFUNCTION(NetMulticast, Reliable)
	void ClientPlaySoundAtLocation(USoundCue* soundCue, FVector location);
	void ClientPlaySoundAtLocation_Implementation(USoundCue* soundCue, FVector location);

	UFUNCTION(NetMulticast, Reliable)
	void ClientPlayAnimMontage(UAnimMontage* animMontage, float playRate = 1.0, FName startSectionName = NAME_None);
	virtual void ClientPlayAnimMontage_Implementation(UAnimMontage* animMontage, float playRate = 1.0, FName startSectionName = NAME_None);

	UFUNCTION(NetMulticast, Reliable)
	void ClientStopAllAnimMontages();
	virtual void ClientStopAllAnimMontages_Implementation();

	UFUNCTION(Client, Reliable)
	void ClientGiveQuest(const FString& questName);
	virtual void ClientGiveQuest_Implementation(const FString& questName);

	UFUNCTION(Client, Reliable)
	void ClientRemoveQuest(const FString& questName);
	virtual void ClientRemoveQuest_Implementation(const FString& questName);

	UFUNCTION(Client, Reliable)
	void ClientUpdateQuestProgress(const FString& questName, int32 newVal);
	virtual void ClientUpdateQuestProgress_Implementation(const FString& questName, int32 newVal);

	UFUNCTION(Client, Reliable)
	void ClientTriggerAbilityCooldown(EPlayerAbilities abilityEnum);
	virtual void ClientTriggerAbilityCooldown_Implementation(EPlayerAbilities abilityEnum);

	UFUNCTION(Client, Reliable)
	void ClientResetAbilityCooldowns();
	virtual void ClientResetAbilityCooldowns_Implementation();

	UFUNCTION(Client, Reliable)
	void ClientOpenLootPanel(AActor* theActor);
	virtual void ClientOpenLootPanel_Implementation(AActor* theActor);

	UFUNCTION(Client, Reliable)
	void ClientCloseLootPanel();
	virtual void ClientCloseLootPanel_Implementation();

	UFUNCTION(Client, Reliable)
	void ClientDisplayErrorMessage(const FString& text);
	virtual void ClientDisplayErrorMessage_Implementation(const FString& text);

	UFUNCTION(Client, Reliable)
	void ClientDisplayNotifyMessage(const FString& text);
	virtual void ClientDisplayNotifyMessage_Implementation(const FString& text);

	UFUNCTION(Client, Reliable)
	void ClientAddFloatingText(const FString& text, FLinearColor textColor, const FString& fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length);
	virtual void ClientAddFloatingText_Implementation(const FString& text, FLinearColor textColor, const FString& fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length);

	UFUNCTION(Client, Reliable)
	void ClientAddFloatingTextEx(const FString& text, FLinearColor textColor, const FString& fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length, bool isRainbow, bool sizePulse, float sizePulseStrength, bool colorPulse, FLinearColor otherColor, bool horizontalPulse, float horizontalPulseStrength);
	virtual void ClientAddFloatingTextEx_Implementation(const FString& text, FLinearColor textColor, const FString& fontPath, FVector2D textScale, FVector2D startPos, FVector2D posOffset, float length, bool isRainbow, bool sizePulse, float sizePulseStrength, bool colorPulse, FLinearColor otherColor, bool horizontalPulse, float horizontalPulseStrength);

	UFUNCTION(Client, Reliable)
	void ClientRelayChatMessage(const FString& theText);
	void ClientRelayChatMessage_Implementation(const FString& theText);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetTarget(AActor* theTarget);
	void ServerSetTarget_Implementation(AActor* theTarget);
	bool ServerSetTarget_Validate(AActor* theTarget);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerPlayerUseAction(int32 index);
	void ServerPlayerUseAction_Implementation(int32 index);
	bool ServerPlayerUseAction_Validate(int32 index);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSendSteamData(const FString& steam64, const FString& legacyID, const FString& personaName, const FString& ipAddress);
	void ServerSendSteamData_Implementation(const FString& steam64, const FString& legacyID, const FString& personaName, const FString& ipAddress);
	bool ServerSendSteamData_Validate(const FString& steam64, const FString& legacyID, const FString& personaName, const FString& ipAddress);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSendChatMessage(const FString& theText);
	void ServerSendChatMessage_Implementation(const FString& theText);
	bool ServerSendChatMessage_Validate(const FString& theText);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerInflictDamage(int32 amount, AActor* theTarget);
	void ServerInflictDamage_Implementation(int32 amount, AActor* theTarget);
	bool ServerInflictDamage_Validate(int32 amount, AActor* theTarget);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSetCombatStatus(bool status);
	void ServerSetCombatStatus_Implementation(bool status);
	bool ServerSetCombatStatus_Validate(bool status);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAttemptLootChest(AActor* theActor, int32 itemID);
	virtual void ServerAttemptLootChest_Implementation(AActor* theActor, int32 itemID);
	virtual bool ServerAttemptLootChest_Validate(AActor* theActor, int32 itemID);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerToggleChest(AActor* theActor);
	virtual void ServerToggleChest_Implementation(AActor* theActor);
	virtual bool ServerToggleChest_Validate(AActor* theActor);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerPickupItem(AActor* theActor);
	virtual void ServerPickupItem_Implementation(AActor* theActor);
	virtual bool ServerPickupItem_Validate(AActor* theActor);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAddInventoryItem(int32 itemID);
	virtual void ServerAddInventoryItem_Implementation(int32 itemID);
	virtual bool ServerAddInventoryItem_Validate(int32 itemID);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerRemoveInventoryItem(int32 itemID);
	virtual void ServerRemoveInventoryItem_Implementation(int32 itemID);
	virtual bool ServerRemoveInventoryItem_Validate(int32 itemID);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerConsumeItem(int32 itemID);
	virtual void ServerConsumeItem_Implementation(int32 itemID);
	virtual bool ServerConsumeItem_Validate(int32 itemID);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerEquipSlotByID(int32 itemID);
	virtual void ServerEquipSlotByID_Implementation(int32 itemID);
	virtual bool ServerEquipSlotByID_Validate(int32 itemID);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerUnEquipSlot(EEquipmentSlot equipSlot);
	virtual void ServerUnEquipSlot_Implementation(EEquipmentSlot equipSlot);
	virtual bool ServerUnEquipSlot_Validate(EEquipmentSlot equipSlot);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerLoadGameData(const FString& steam64);
	virtual void ServerLoadGameData_Implementation(const FString& steam64);
	virtual bool ServerLoadGameData_Validate(const FString& steam64);

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSaveGameData(const FString& steam64, const FString& personaName);
	virtual void ServerSaveGameData_Implementation(const FString& steam64, const FString& personaName);
	virtual bool ServerSaveGameData_Validate(const FString& steam64, const FString& personaName);

protected:

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

	UFUNCTION()
	virtual void PostNetInit() override;

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Destroyed() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION()
	virtual void Jump() override;

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Get the clicked Actor */
	AActor* GetClickedActor();

	/** Custom Input Events */
	void MouseWheelUp();

	void MouseWheelDown();

	void PressedLeftControl();

	void PressedLeftClick();

	void ReleasedLeftClick();

	void PressedRightClick();

	void ReleasedRightClick();

	void PressedActionKey(int slot);
};

