// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
class AWornEquipmentActorParent;

#include "StaticFunctions.generated.h"

UCLASS()
class RPGDEMO_API UStaticFunctions : public UObject
{
	GENERATED_BODY()

	public:

		static TMap<int32, FItemDetails> m_ItemMap;
		static TMap<int32, EEquipmentSlot> m_EquipmentSlotMap;
		static TMap<int32, FString> m_EquipmentClassMap;
		static TMap<int32, FConsumableData> m_ConsumableDataMap;
		static TMap<int32, FArmorStats> m_ArmorStatsMap;
		static TMap<int32, FWeaponStats> m_WeaponStatsMap;
		static TMap<int32, FWornEquipmentData> m_WornEquipmentDataMap;
		static TMap<ETargetTypes, FTargetData> m_TargetDataMap;
		static TMap<EPlayerAbilities, FAbilityData> m_AbilityMap;
		static TMap<FString, FQuestDetails> m_QuestMap;
		static TMap<FString, UTexture2D*> m_TextureCache;
		static TMap<FString, UFont*> m_FontCache;
		static TMap<FString, UMaterial*> m_MaterialCache;
		static TMap<FString, UStaticMesh*> m_StaticMeshCache;
		static TArray<FString> m_RandomNames;

		static void InitializeData();
		static void GenerateWeaponDetails(FString itemName, EEquipmentSlot itemSlot, FString itemDescription, int32 attackPoints, float attackSpeed, FName socketName, FVector offset, FVector rotation, FVector scale, const FString& texturePath, const FString& meshPath);
		static void GenerateArmorDetails(FString itemName, EEquipmentSlot itemSlot, FString itemDescription, int32 attackPoints, int32 defensePoints, int32 healthPoints, int32 energyPoints, FName socketName, FVector offset, FVector rotation, FVector scale, const FString& texturePath, const FString& meshPath);
		static void GenerateConsumableDetails(FString itemName, FString itemDescription, EConsumeType consumeType, int32 pointAmount, float cooldownTime, const FString& texturePath);
		static void GenerateItemDetails(FString itemName, FString itemDescription, EItemType itemType, const FString& texturePath);
		static void GenerateTargetData(ETargetTypes targetType, FString targetName, const FString& iconPath);
		static void GenerateAbilityData(EPlayerAbilities abilityEnum, FString name, FString description, float cooldown, FString texturePath);
		static void GenerateSlayingQuestData(FString questName, FString questDescription, int32 requiredKills, FString enemyFaction, int32 xpReward, TArray<FString> itemRewards);
		static void GenerateItemRetrievalQuestData(FString questName, FString questDescription, int32 requiredItems, FString itemName, int32 xpReward, TArray<FString> itemRewards);
		static void InitializeItemData();
		static void InitializeTargetData();
		static void InitializeAbilityData();
		static void InitializeQuestData();
		static int32 GetItemIDByName(FString itemName);
		static bool ItemIDExists(int32 itemID);
		static EEquipmentSlot GetEquipmentSlotByID(int32 itemID);
		static void SetEquipmentSlot(AActor* theActor, EEquipmentSlot equipSlot, AActor* theItemActor);
		static AWornEquipmentActorParent* GetEquipmentSlot(AActor* theActor, EEquipmentSlot equipSlot);
		static UClass* GetEquipmentClassByName(UWorld* theWorld, FString itemName);
		static void DebugMessage(FColor displayColor, const FString& message);
		static UTexture2D* LoadTextureFromPath(const FString& aPath);
		static UMaterial* LoadMaterialFromPath(const FString& aPath);
		static FLinearColor RainbowStrobe(UWorld* theWorld, float frequency, float alpha);
		static UFont* LoadFontFromPath(const FString& aPath);
		static UStaticMesh* LoadStaticMeshFromPath(const FString& aPath);
		static UParticleSystem* LoadParticleSystemFromPath(const FString& aPath);
		static USoundCue* LoadSoundCueFromPath(const FString& aPath);
		static bool ActorIsFacingLocation(AActor* theActor, FVector theLocation);
		static FString IntIPAddressToString(uint32 intAddress);
		template <class T>
		static T RandomArrayValue(TArray<T> theArray);
		static FString GetRandomName();
		static FWeaponStats GetWeaponStats(AActor* theActor);
		static FArmorStats GetArmorStats(AActor* theActor);
		static FEquipmentStats GetEquipmentStats(AActor* theActor);
		static FString SteamID64ToLegacy(uint64 steam64);
		static void SaveLoadData(FArchive& archive, FPlayerData& playerData, TArray<int32>& inventoryArray, TArray<int32>& equipmentArray);
		static void SaveGameDataToPath(const FString& fullPath, FBufferArchive& ToBinary, FPlayerData& playerData, TArray<int32>& inventoryArray, TArray<int32>& equipmentArray);
		static void LoadGameDataFromPath(const FString& fullPath, FPlayerData& playerData, TArray<int32>& inventoryArray, TArray<int32>& equipmentArray);
		static TArray<int32> GetEquipmentArray(FEquipmentSlots& playerEquipment);
		static void LoadEquipmentArray(TArray<int32>& equipmentArray, AActor* thePlayer);
		static FString UInt64ToFString(uint64 theInt);
		static FString SplitStringToLines(FString aString, int32 lineCharacterAmount);
		static EPlayerAbilities IndexToActionSlot(int index, AActor* theActor);
		static void ApplyForce(AActor* theActor, FVector dir, float force);
		static void SetParticleEmitterParameters(UParticleSystemComponent* particleSystem, TArray<FParticleParameter> parameterArray);
		static void SetObjectForDestruction(UWorld* theWorld, UObject* theObject, float length);
		static void TriggerDelayedAction(UWorld* theWorld, FTimerDelegate timerDelegate, float length, bool shouldLoop = false);
		static float GetActorHeight(AActor* theActor);
		static FPlayerUMGPosition GetDefinedUMGPosition(UClass* widgetClass, TArray<FPlayerUMGPosition> positionArray);
		static FViewportData GetViewportData();
		static FVector GetActorCollisionBounds(AActor* theActor);
};
