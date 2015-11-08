// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"

class AWornEquipmentActorParent;
class UCustomUserWidgetParent;
#include "StaticStructs.generated.h"

DECLARE_DELEGATE(FAbilityFunctionDelegate);
DECLARE_DELEGATE(FItemFunctionDelegate);

USTRUCT(BlueprintType)
struct RPGDEMO_API FQuestLoot
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	float DropChance;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FViewportData
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	FVector2D ViewportSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	float ViewportScale;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FPlayerUMGPosition
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UClass* WidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	FVector2D ViewportRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	FVector2D ViewportOffset;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FItemRetrievalQuestDetails
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	int32 CurrentItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	int32 RequiredItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	FString ItemName;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FSlayingQuestDetails
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	int32 CurrentKills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	int32 RequiredKills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	FString FactionName;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FQuestDetails
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	EQuestTypes Type;

	FItemRetrievalQuestDetails ItemRetrievalProgress;

	FSlayingQuestDetails SlayingProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	int32 XPReward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	TArray<FString> ItemRewards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quests)
	bool Completed;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FParticleParameter
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	FName ParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	EParticleParameterTypes ParameterType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	float ScalarValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	FVector VectorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	FLinearColor ColorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	AActor* ActorValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	UMaterial* MaterialValue;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FPlayerAnimMontages
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* AttackTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* Block;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* Death;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* DrainLife;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* HeavyAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* Hurt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* Knockout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* Loot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* Pushback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	UAnimMontage* Strikedown;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FAbilityData
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	float Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ability)
	FString TexturePath;

	FAbilityFunctionDelegate AbilityDelegate;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FPlayerActionKeys
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeyOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeyTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeyThree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeyFour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeyFive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeySix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeySeven;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeyEight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	EPlayerAbilities ActionKeyNine;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FUMGWidgetClasses
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> TargetWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> EquipmentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> InventoryOptionsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> CharacterBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> ChatboxWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> PlayerActionBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> LootWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> LootSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> PlayerVitalsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> QuestLogWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> QuestLogSlotWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	TSubclassOf<UCustomUserWidgetParent> AbilityInfoWidget;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FPlayerUMGWidgets
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* TargetWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* EquipmentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* InventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* InventoryOptionsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* CharacterBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* ChatboxWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* PlayerActionBarWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* LootWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* PlayerVitalsWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UMG)
	UCustomUserWidgetParent* QuestLogWidget;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FTargetData
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Target)
	FString TargetName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Target)
	FString IconPath;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FPlayerData
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	FString DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	FString Rank;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	int32 Experience;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FConsumableData
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EConsumeType ConsumeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 PointAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float CooldownTime;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FWornEquipmentData
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FVector Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FVector Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FVector Scale;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FItemDetails
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FString ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FString ItemTexturePath;

	FItemFunctionDelegate UseDelegate;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FEquipmentStats
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 AttackPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 DefensePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 HealthPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	int32 EnergyPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float AttackSpeed;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FArmorStats
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 AttackPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 DefensePoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 HealthPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 EnergyPoints;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FWeaponStats
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 AttackPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	float AttackSpeed;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FEquipmentSlots
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	AWornEquipmentActorParent* HeadSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	AWornEquipmentActorParent* LeftShoulderSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	AWornEquipmentActorParent* RightShoulderSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	AWornEquipmentActorParent* ChestSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	AWornEquipmentActorParent* BackSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	AWornEquipmentActorParent* LeftHandSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	AWornEquipmentActorParent* RightHandSlot;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FEquipmentBlueprint
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	TSubclassOf<AActor> ItemActor;
};

USTRUCT(BlueprintType)
struct RPGDEMO_API FFloatingText
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	FLinearColor TextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	UFont* TextFont;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	FVector2D TextScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	FVector2D StartPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	FVector2D PosOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	float EndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	bool IsRainbow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	bool SizePulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	float SizePulseStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	bool ColorPulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	FLinearColor OtherColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	bool HorizontalPulse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating Text")
	float HorizontalPulseStrength;
};

UCLASS()
class RPGDEMO_API UStaticStructs : public UObject
{
	GENERATED_BODY()
};
