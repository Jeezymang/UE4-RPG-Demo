// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "StaticEnums.generated.h"

UENUM(BlueprintType)
enum class EQuestTypes : uint8
{
	Slaying UMETA(DisplayName = "Slaying"),
	ItemRetrieval UMETA(DisplayName = "ItemRetrieval")
};

UENUM(BlueprintType)
enum class EParticleParameterTypes : uint8
{
	Scalar UMETA(DisplayName = "Scalar"),
	Vector UMETA(DisplayName = "Vector"),
	Color UMETA(DisplayName = "Color"),
	Actor UMETA(DisplayName = "Actor"),
	Material UMETA(DisplayName = "Material")
};

UENUM(BlueprintType)
enum class EPlayerAbilities : uint8
{
	None UMETA(DisplayName = "None"),
	HeavyAttack UMETA(DisplayName = "Heavy Attack"),
	Knockout UMETA(DisplayName = "Knockout"),
	WeakPushback UMETA(DisplayName = "Pushback"),
	WeakLifeDrain UMETA(DisplayName = "Weak Life Drain"),
	Strikedown UMETA(DisplayName = "Strikedown")
};

UENUM(BlueprintType)
enum class ETargetTypes : uint8
{
	Character UMETA(DisplayName = "Character"),
	Murloc UMETA(DisplayName = "Murloc"),
	Dragonoid UMETA(DisplayName = "Dragonoid")
};

UENUM(BlueprintType)
enum class EBotStates : uint8
{
	Patrol UMETA(DisplayName = "Patrol"),
	Chasing UMETA(DisplayName = "Chasing"),
	Attacking UMETA(DisplayName = "Attacking"),
	Fleeing UMETA(DisplayName = "Fleeing")
};

UENUM(BlueprintType)
enum class EConsumeType : uint8
{
	Health UMETA(DisplayName = "Health"),
	Energy UMETA(DisplayName = "Energy"),
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Consumable UMETA(DisplayName = "Consumable"),
	QuestItem UMETA(DisplayName = "QuestItem")
};

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Head UMETA(DisplayName = "Head"),
	LeftShoulder UMETA(DisplayName = "Left Shoulder"),
	RightShoulder UMETA(DisplayName = "Right Shoulder"),
	Chest UMETA(DisplayName = "Chest"),
	Back UMETA(DisplayName = "Back"),
	LeftHand UMETA(DisplayName = "Left Hand"),
	RightHand UMETA(DisplayName = "Right Hand")
};

UENUM(BlueprintType)
enum class EPlayerAnimState : uint8
{
	None UMETA(DisplayName = "None"),
	Attack UMETA(DisplayName = "Attacking"),
	Block UMETA(DisplayName = "Block"),
	Hurt UMETA(DisplayName = "Hurt"),
	Death UMETA(DisplayName = "Death"),
	Sheathe UMETA(DisplayName = "Sheathe"),
	Stun UMETA(DisplayName = "Stun"),
	Loot UMETA(DisplayName = "Loot")
};

UCLASS()
class RPGDEMO_API UStaticEnums : public UObject
{
	GENERATED_BODY()
};
