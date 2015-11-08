// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "Game/WornEquipmentActorParent.h"
#include "StaticFunctions.h"
#include "RPGDemoGameMode.h"
#include "RPGDemoCharacter.h"
#include "Json.h"
#include <sstream>
#include <string>
#include "Archive.h"
#include "ArchiveBase.h"
#include "FileManager.h"
#include "Runtime/Engine/Classes/Engine/UserInterfaceSettings.h"
#include "Runtime/Engine/Classes/Engine/RendererSettings.h"

TMap<int32, FItemDetails> UStaticFunctions::m_ItemMap = TMap<int32, FItemDetails>();
TMap<int32, EEquipmentSlot> UStaticFunctions::m_EquipmentSlotMap = TMap<int32, EEquipmentSlot>();
TMap<int32, FString> UStaticFunctions::m_EquipmentClassMap = TMap<int32, FString>();
TMap<int32, FConsumableData> UStaticFunctions::m_ConsumableDataMap = TMap<int32, FConsumableData>();
TMap<int32, FArmorStats> UStaticFunctions::m_ArmorStatsMap = TMap<int32, FArmorStats>();
TMap<int32, FWeaponStats> UStaticFunctions::m_WeaponStatsMap = TMap<int32, FWeaponStats>();
TMap<int32, FWornEquipmentData> UStaticFunctions::m_WornEquipmentDataMap = TMap<int32, FWornEquipmentData>();
TMap<ETargetTypes, FTargetData> UStaticFunctions::m_TargetDataMap = TMap<ETargetTypes, FTargetData>();
TMap<EPlayerAbilities, FAbilityData> UStaticFunctions::m_AbilityMap = TMap<EPlayerAbilities, FAbilityData>();
TMap<FString, FQuestDetails> UStaticFunctions::m_QuestMap = TMap<FString, FQuestDetails>();
TMap<FString, UStaticMesh*> UStaticFunctions::m_StaticMeshCache = TMap<FString, UStaticMesh*>();
TMap<FString, UFont*> UStaticFunctions::m_FontCache = TMap<FString, UFont*>();
TMap<FString, UMaterial*> UStaticFunctions::m_MaterialCache = TMap<FString, UMaterial*>();
TMap<FString, UTexture2D*> UStaticFunctions::m_TextureCache = TMap<FString, UTexture2D*>();
TArray<FString> UStaticFunctions::m_RandomNames = TArray<FString>();

void UStaticFunctions::InitializeData()
{
	if (m_ItemMap.Num() > 0) return;
	FString anArray[] = { FString("Al"), FString("Archie"), FString("Brooke"), FString("Roy"), FString("Glenda"), FString("Lucas"), FString("Shawn"), FString("Elijah"), FString("Jean"), FString("Elisa"), FString("Rene"), FString("Moses"), FString("Cary"), FString("Carlton"), FString("Rex"), FString("Bill"), FString("Laura"), FString("Forrest"), FString("Wilma"), FString("Essie"), FString("Albert"), FString("Raquel"), FString("Fannie"), FString("Everett"),
		FString("Lucia"), FString("Ernesto"), FString("Victoria"), FString("Yvonne"), FString("Samuel"), FString("Shaun"), FString("Angelica"), FString("May"), FString("Lila"), FString("Steven"), FString("Caroline"), FString("Norma"), FString("Erika"),
		FString("Hattie"), FString("Tom"), FString("Josh"), FString("Maurice"), FString("Domingo"), FString("Lance"), FString("Kristin"), FString("Joy"), FString("Florence"), FString("Hope"), FString("Katrina"), FString("Chad"), FString("Hilda") };
	m_RandomNames.Append(anArray, ARRAY_COUNT(anArray));
	InitializeItemData();
	DebugMessage(FColor::Blue, "Generated " + FString::FromInt(m_ItemMap.Num()) + " items.");
	InitializeTargetData();
	InitializeAbilityData();
	InitializeQuestData();
}

void UStaticFunctions::InitializeItemData()
{
	GenerateWeaponDetails("Sword", EEquipmentSlot::RightHand, "Looks like a dull sword.", 5, 2, TEXT("RightHand_Socket"), FVector(-14.83, 7.68, 8.11), FVector(0, 0, 0), FVector(0.2, 0.2, 0.2), "/Game/Textures/Icons/T_InvSlotSword", "/Game/Meshes/Weapons/LowPolySword");
	GenerateWeaponDetails("Fel Axe", EEquipmentSlot::RightHand, "Seems rather eerie, spooky.", 7, 2, TEXT("RightHand_Socket"), FVector(5.86, -0.76, -13.7), FVector(170, 90, -30), FVector(0.3, 0.3, 0.3), "/Game/Textures/Icons/T_InvSlotFelAxe", "/Game/Meshes/Weapons/FelAxe");
	GenerateWeaponDetails("Jinyu Sword", EEquipmentSlot::RightHand, "Has a really nice look to it.", 10, 1.25, TEXT("RightHand_Socket"), FVector(1.76, 6, 8.1), FVector(0, 90, 0), FVector(1.2, 1.2, 1.2), "/Game/Textures/Icons/T_InvSlotJinyuSword", "/Game/Meshes/Weapons/JinyuSword");
	GenerateWeaponDetails("Spiked Mace", EEquipmentSlot::RightHand, "Looks like it can do some damage.", 10, 2, TEXT("RightHand_Socket"), FVector(5.86, -0.76, -13.7), FVector(170, 90, -30), FVector(0.3, 0.3, 0.3), "/Game/Textures/Icons/T_InvSlotSpikedMace", "/Game/Meshes/Weapons/SpikedMace");
	GenerateWeaponDetails("Crystal Wand", EEquipmentSlot::RightHand, "Seems to have some sort of enchantment.", 8, 2, TEXT("RightHand_Socket"), FVector(0.45, -1.46, -11.53), FVector(170, 90, -30), FVector(0.7, 0.7, 0.7), "/Game/Textures/Icons/T_InvSlotCrystalWand", "/Game/Meshes/Weapons/CrystalWand");
	GenerateArmorDetails("Viking Helmet", EEquipmentSlot::Head, "Reminds you of the Vikings.", 1, 2, 0, 3, TEXT("Head_Socket"), FVector(0, 0, 11.49), FVector(0, -90, 0), FVector(0.06, 0.06, 0.06), "/Game/Textures/Icons/T_InvSlotVikingHelmet", "/Game/Meshes/Armor/VikingHelmet");
	GenerateArmorDetails("Warrior's Helmet", EEquipmentSlot::Head, "Makes you look tough.", 2, 3, 10, 0, TEXT("Head_Socket"), FVector(4.46, 0, 0.7), FVector(270, 0, 90), FVector(0.3, 0.3, 0.3), "/Game/Textures/Icons/T_InvSlotWarriorsHelmet", "/Game/Meshes/Armor/WarriorsHelmet");
	GenerateArmorDetails("Spiked Faceguard", EEquipmentSlot::Head, "Looks uncomfortable to wear.", 1, 2, 0, 3, TEXT("Head_Socket"), FVector(1.26, 0, -2.72), FVector(90, 0, -40.8), FVector(0.55, 0.55, 0.55), "/Game/Textures/Icons/T_InvSlotSpikedFaceguard", "/Game/Meshes/Armor/SpikedFaceguard");
	GenerateArmorDetails("Wizard's Hat", EEquipmentSlot::Head, "Looks like a Wizard would wear it.", 1, 2, 0, 3, TEXT("Head_Socket"), FVector(0, 5.58, 9.6), FVector(-15.29, -90, 0), FVector(0.65, 0.65, 0.65), "/Game/Textures/Icons/T_InvSlotWizardsHat", "/Game/Meshes/Armor/WizardsHat");
	GenerateArmorDetails("Spartan Shield", EEquipmentSlot::LeftHand, "Seems like a dependable shield.", 0, 10, 0, 0, TEXT("LeftHand_Socket"), FVector(5.1, 5.4, 0), FVector(0, 0, 81.4), FVector(0.3, 0.3, 0.3), "/Game/Textures/Icons/T_InvSlotSpartanShield", "/Game/Meshes/Armor/SpartanShield");
	GenerateArmorDetails("Outland Shield", EEquipmentSlot::LeftHand, "Looks rather ancient.", 0, 20, 0, 0, TEXT("LeftHand_Socket"), FVector(7.16, -0.01, 0), FVector(0, 0, 165.75), FVector(0.3, 0.3, 0.3), "/Game/Textures/Icons/T_InvSlotOutlandShield", "/Game/Meshes/Armor/OutlandShield");
	GenerateArmorDetails("Lich Shield", EEquipmentSlot::LeftHand, "Seems like it's harboring some sort of darkness.", 0, 20, 0, 0, TEXT("LeftHand_Socket"), FVector(3.55, 0, 15.6), FVector(90, 0, 82), FVector(0.9, 0.9, 0.9), "/Game/Textures/Icons/T_InvSlotLichShield", "/Game/Meshes/Armor/LichShield");
	GenerateArmorDetails("Dragonwing Shield", EEquipmentSlot::LeftHand, "Looks like it may have came from a dragon.", 0, 20, 0, 0, TEXT("LeftHand_Socket"), FVector(8.65, -2.77, 5.97), FVector(0, 0, -190), FVector(0.5, 0.5, 0.5), "/Game/Textures/Icons/T_InvSlotDragonwingShield", "/Game/Meshes/Armor/DragonwingShield");
	GenerateArmorDetails("Right Leather Shoulder", EEquipmentSlot::RightShoulder, "Doesn't look comfortable.", 0, 3, 0, 0, TEXT("RightShoulder_Socket"), FVector(0, 0, 5.5), FVector(120, 0, 270), FVector(0.35, 0.35, 0.35), "/Game/Textures/Icons/T_InvSlotRightLeatherShoulder", "/Game/Meshes/Armor/RightRobeShoulder");
	GenerateArmorDetails("Right Plate Shoulder", EEquipmentSlot::RightShoulder, "Seems to be pretty sturdy.", 0, 3, 0, 0, TEXT("RightShoulder_Socket"), FVector(0.47, 7.76, 5.5), FVector(160.11, -31.73, 270), FVector(0.4, 0.4, 0.4), "/Game/Textures/Icons/T_InvSlotRightPlateShoulder", "/Game/Meshes/Armor/RightPlateShoulder");
	GenerateArmorDetails("Left Feathered Shoulder", EEquipmentSlot::LeftShoulder, "Looks rather fashionable.", 0, 3, 0, 0, TEXT("LeftShoulder_Socket"), FVector(0, -4.83, -0.48), FVector(68.48, 0, 90), FVector(0.4, 0.4, 0.4), "/Game/Textures/Icons/T_InvSlotLeftFeatheredShoulder", "/Game/Meshes/Armor/LeftFeatheredShoulder");
	GenerateArmorDetails("Left Leather Shoulder", EEquipmentSlot::LeftShoulder, "Doesn't look comfortable.", 0, 3, 0, 0, TEXT("LeftShoulder_Socket"), FVector(0, 0, 5.5), FVector(-60, 0, 270), FVector(0.35, 0.35, 0.35), "/Game/Textures/Icons/T_InvSlotLeftLeatherShoulder", "/Game/Meshes/Armor/RightRobeShoulder");
	GenerateArmorDetails("Left Plate Shoulder", EEquipmentSlot::LeftShoulder, "Seems to be pretty sturdy.", 0, 3, 0, 0, TEXT("LeftShoulder_Socket"), FVector(-2.26, 6.89, 7.10), FVector(-20, 0, 275), FVector(0.4, 0.4, 0.4), "/Game/Textures/Icons/T_InvSlotLeftPlateShoulder", "/Game/Meshes/Armor/RightPlateShoulder");
	GenerateArmorDetails("Small Skull Chestpiece", EEquipmentSlot::Chest, "Doesn't look like it'd offer much protection.", 0, 3, 0, 0, TEXT("Chest_Socket"), FVector(13.1, 0, 5.5), FVector(270, 0, 90), FVector(0.6, 0.6, 0.6), "/Game/Textures/Icons/T_InvSlotSmallSkullChestpiece", "/Game/Meshes/Armor/SmallSkullChestpiece");
	GenerateConsumableDetails("Burger", "Looks pretty tasty.", EConsumeType::Health, 15, 60, "/Game/Textures/Icons/T_InvSlotBurger");
	GenerateConsumableDetails("Water Bottle", "Seems like it'd be refreshing.", EConsumeType::Energy, 25, 60, "/Game/Textures/Icons/T_InvSlotWater");
	GenerateConsumableDetails("Health Potion", "Restores a decent amount of health.", EConsumeType::Health, 35, 60, "/Game/Textures/Icons/T_InvSlotHealthPotion");
	GenerateItemDetails("Murloc Fin", "It still seems to be moving around a bit, disgusting.", EItemType::QuestItem, "/Game/Textures/Icons/T_InvSlotMurlocFin");
	GenerateItemDetails("Dragon Scales", "Looks like they may be worth a decent amount.", EItemType::QuestItem, "/Game/Textures/Icons/T_InvSlotDragonScales");
}

void UStaticFunctions::InitializeTargetData()
{
	GenerateTargetData(ETargetTypes::Character, "Character", "/Game/Textures/Icons/T_TargetPlayer");
	GenerateTargetData(ETargetTypes::Murloc, "Murloc", "/Game/Textures/Icons/T_TargetMurloc");
	GenerateTargetData(ETargetTypes::Dragonoid, "Dragonoid", "/Game/Textures/Icons/T_TargetDragonoid");
}

void UStaticFunctions::InitializeAbilityData()
{
	GenerateAbilityData(EPlayerAbilities::HeavyAttack, "Heavy Attack", "Hits the target hard.", 15, "/Game/Textures/Icons/T_ActionSlotHeavyAttack");
	GenerateAbilityData(EPlayerAbilities::Knockout, "Knockout", "Stuns the target for a brief amount of time.", 25, "/Game/Textures/Icons/T_ActionSlotKnockout");
	GenerateAbilityData(EPlayerAbilities::WeakPushback, "Weak Pushback", "Knocks the target back slightly.", 25, "/Game/Textures/Icons/T_ActionSlotWeakPushback");
	GenerateAbilityData(EPlayerAbilities::WeakLifeDrain, "Weak Life Drain", "Drains a little bit of the target's health.", 15, "/Game/Textures/Icons/T_ActionSlotWeakLifeDrain");
	GenerateAbilityData(EPlayerAbilities::Strikedown, "Strikedown", "Does decent damage to the target.", 10, "/Game/Textures/Icons/T_ActionSlotStrikedown");
}

void UStaticFunctions::InitializeQuestData()
{
	TArray<FString> killMurlocsRewards;
	killMurlocsRewards.Add("Fel Axe");
	GenerateSlayingQuestData("Slay Some Murlocs", "There's an abundance of Murlocs wandering around here. Kill five of them.", 5, "Murlocs", 200, killMurlocsRewards);
	TArray<FString> getBurgersRewards;
	getBurgersRewards.Add("Water Bottle");
	GenerateItemRetrievalQuestData("Get Burgers", "You're starving, you should go try to find three Burgers.", 3, "Burger", 100, getBurgersRewards);
	TArray<FString> murlocFinRewards;
	GenerateItemRetrievalQuestData("Murloc Fins", "You've been asked to slay a few Murlocs and collect their fins.", 5, "Murloc Fin", 125, murlocFinRewards);
	TArray<FString> dragonScalesRewards;
	GenerateItemRetrievalQuestData("Dragon Scales", "Slay a few Dragonoids and retrieve their scales, they're said to be worth quite a bit.", 10, "Dragon Scales", 200, dragonScalesRewards);
	TArray<FString> killDragonRewards;
	GenerateSlayingQuestData("Slay Some Dragonoids", "There have been some Dragonoids spotted in the area, you've been asked to slay some.", 5, "Dragonoids", 200, killDragonRewards);
}

void UStaticFunctions::LoadEquipmentArray(TArray<int32>& equipmentArray, AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer) return;
	for (int i = 0; i < equipmentArray.Num(); i++)
	{
		if (equipmentArray[i] == -1) continue;
		thePlayer->AttachEquipment(equipmentArray[i]);
	}
}

TArray<int32> UStaticFunctions::GetEquipmentArray(FEquipmentSlots& playerEquipment)
{
	TArray<int32> equipmentArray;
	if (playerEquipment.HeadSlot)
		equipmentArray.Add(UStaticFunctions::GetItemIDByName(playerEquipment.HeadSlot->m_ItemName));
	else
		equipmentArray.Add(-1);
	if (playerEquipment.BackSlot)
		equipmentArray.Add(UStaticFunctions::GetItemIDByName(playerEquipment.BackSlot ->m_ItemName));
	else
		equipmentArray.Add(-1);
	if (playerEquipment.ChestSlot)
		equipmentArray.Add(UStaticFunctions::GetItemIDByName(playerEquipment.ChestSlot->m_ItemName));
	else
		equipmentArray.Add(-1);
	if (playerEquipment.RightShoulderSlot)
		equipmentArray.Add(UStaticFunctions::GetItemIDByName(playerEquipment.RightShoulderSlot->m_ItemName));
	else
		equipmentArray.Add(-1);
	if (playerEquipment.LeftShoulderSlot)
		equipmentArray.Add(UStaticFunctions::GetItemIDByName(playerEquipment.LeftShoulderSlot->m_ItemName));
	else
		equipmentArray.Add(-1);
	if (playerEquipment.RightHandSlot)
		equipmentArray.Add(UStaticFunctions::GetItemIDByName(playerEquipment.RightHandSlot->m_ItemName));
	else
		equipmentArray.Add(-1);
	if (playerEquipment.LeftHandSlot)
		equipmentArray.Add(UStaticFunctions::GetItemIDByName(playerEquipment.LeftHandSlot->m_ItemName));
	else
		equipmentArray.Add(-1);
	return equipmentArray;
}

void UStaticFunctions::LoadGameDataFromPath(const FString& fullPath, FPlayerData& playerData, TArray<int32>& inventoryArray, TArray<int32>& equipmentArray)
{
	TArray<uint8> binaryArray;
	if (!FFileHelper::LoadFileToArray(binaryArray, *fullPath))
	{
		DebugMessage(FColor::Red, "Invalid file, load failed.");
		return;
	}
	DebugMessage(FColor::Red, "Loaded File Size.");
	DebugMessage(FColor::Red, FString::FromInt(binaryArray.Num()));

	if (binaryArray.Num() <= 0) return;

	FMemoryReader fromBinary = FMemoryReader(binaryArray, true);
	fromBinary.Seek(0);
	SaveLoadData(fromBinary, playerData, inventoryArray, equipmentArray);

	fromBinary.FlushCache();
	binaryArray.Empty();
	//fromBinary.Close();
}

void UStaticFunctions::SaveGameDataToPath(const FString& fullPath, FBufferArchive& ToBinary, FPlayerData& playerData, TArray<int32>& inventoryArray, TArray<int32>& equipmentArray)
{
	SaveLoadData(ToBinary, playerData, inventoryArray, equipmentArray);

	if (ToBinary.Num() <= 0) return;

	if (FFileHelper::SaveArrayToFile(ToBinary, *fullPath))
	{
		ToBinary.FlushCache();
		ToBinary.Empty();
		DebugMessage(FColor::Red, "Save successful.");
		return;
	}
	ToBinary.FlushCache();
	ToBinary.Empty();
	DebugMessage(FColor::Red, "Save failed");
}

void UStaticFunctions::SaveLoadData(FArchive& archive, FPlayerData& playerData, TArray<int32>& inventoryArray, TArray<int32>& equipmentArray)
{
	archive << playerData.DisplayName;
	archive << playerData.Rank;
	archive << playerData.Level;
	archive << playerData.Experience;
	archive << equipmentArray;
	archive << inventoryArray;
}

FString UStaticFunctions::SteamID64ToLegacy(uint64 steam64)
{
	uint64 STEAM_BASELINE = 76561197960265728;
	uint64 segmentTwo = (steam64 - STEAM_BASELINE) / 2;
	uint64 segmentOne = steam64 % 2;
	std::ostringstream segmentTwoStringStream;
	segmentTwoStringStream << segmentTwo;
	std::string segmentTwoString(segmentTwoStringStream.str());
	std::ostringstream segmentOneStringStream;
	segmentOneStringStream << segmentOne;
	std::string segmentOneString(segmentOneStringStream.str());
	FString segmentTwoFString = segmentTwoString.c_str();
	FString segmentOneFString = segmentOneString.c_str();
	FString output = "STEAM_0:" + segmentOneFString + ":" + segmentTwoFString;
	return output;
}

template <class T>
T UStaticFunctions::RandomArrayValue(TArray<T> theArray)
{
	return theArray[FMath::RandRange(0, theArray.Num() - 1)];
}

FString UStaticFunctions::GetRandomName()
{
	return RandomArrayValue(m_RandomNames);
}

FWeaponStats UStaticFunctions::GetWeaponStats(AActor* theActor)
{
	FWeaponStats blankStruct = FWeaponStats();
	blankStruct.AttackPoints = 0;
	blankStruct.AttackSpeed = 1;
	AWornEquipmentActorParent* theItem = Cast<AWornEquipmentActorParent>(theActor);
	if (theItem)
	{
		int itemID = GetItemIDByName(theItem->m_ItemName);
		if (m_WeaponStatsMap.Contains(itemID))
		{
			return (FWeaponStats)m_WeaponStatsMap[itemID];
		}
		else
			return blankStruct;
	}
	else
		return blankStruct;
}

FArmorStats UStaticFunctions::GetArmorStats(AActor* theActor)
{
	FArmorStats blankStruct = FArmorStats();
	blankStruct.AttackPoints = 0;
	blankStruct.DefensePoints = 0;
	blankStruct.HealthPoints = 0;
	blankStruct.EnergyPoints = 0;
	AWornEquipmentActorParent* theItem = Cast<AWornEquipmentActorParent>(theActor);
	if (theItem)
	{
		int itemID = GetItemIDByName(theItem->m_ItemName);
		if (m_ArmorStatsMap.Contains(itemID))
		{
			return (FArmorStats)m_ArmorStatsMap[itemID];
		}
		else
			return blankStruct;
	}
	else
		return blankStruct;
}

void AddArmorStats(FArmorStats statsStruct, int32& attackPoints, int32& defensePoints, int32& healthPoints, int32& energyPoints)
{
	attackPoints += statsStruct.AttackPoints;
	defensePoints += statsStruct.DefensePoints;
	healthPoints += statsStruct.HealthPoints;
	energyPoints += statsStruct.EnergyPoints;
}

FEquipmentStats UStaticFunctions::GetEquipmentStats(AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer) return FEquipmentStats();
	int32 attackPoints = 0;
	int32 defensePoints = 0;
	int32 healthPoints = 0;
	int32 energyPoints = 0;
	float attackSpeed = 1;
	AWornEquipmentActorParent* headActor = GetEquipmentSlot(theActor, EEquipmentSlot::Head);
	AddArmorStats(GetArmorStats(headActor), attackPoints, defensePoints, healthPoints, energyPoints);
	AWornEquipmentActorParent* backActor = GetEquipmentSlot(theActor, EEquipmentSlot::Back);
	AddArmorStats(GetArmorStats(backActor), attackPoints, defensePoints, healthPoints, energyPoints);
	AWornEquipmentActorParent* chestActor = GetEquipmentSlot(theActor, EEquipmentSlot::Chest);
	AddArmorStats(GetArmorStats(chestActor), attackPoints, defensePoints, healthPoints, energyPoints);
	AWornEquipmentActorParent* rightShoulderActor = GetEquipmentSlot(theActor, EEquipmentSlot::RightShoulder);
	AddArmorStats(GetArmorStats(rightShoulderActor), attackPoints, defensePoints, healthPoints, energyPoints);
	AWornEquipmentActorParent* leftShoulderActor = GetEquipmentSlot(theActor, EEquipmentSlot::LeftShoulder);
	AddArmorStats(GetArmorStats(leftShoulderActor), attackPoints, defensePoints, healthPoints, energyPoints);
	AWornEquipmentActorParent* leftHandActor = GetEquipmentSlot(theActor, EEquipmentSlot::LeftHand);
	AddArmorStats(GetArmorStats(leftHandActor), attackPoints, defensePoints, healthPoints, energyPoints);
	AWornEquipmentActorParent* rightHandActor = GetEquipmentSlot(theActor, EEquipmentSlot::RightHand);
	FWeaponStats weaponStats = GetWeaponStats(rightHandActor);
	attackPoints += weaponStats.AttackPoints;
	attackSpeed = weaponStats.AttackSpeed;
	FEquipmentStats statsStruct = FEquipmentStats();
	statsStruct.AttackPoints = attackPoints;
	statsStruct.DefensePoints = defensePoints;
	statsStruct.HealthPoints = healthPoints;
	statsStruct.EnergyPoints = energyPoints;
	statsStruct.AttackSpeed = attackSpeed;
	return statsStruct;
}

FString UStaticFunctions::UInt64ToFString(uint64 theInt)
{
	std::ostringstream stringStream;
	stringStream << theInt;
	std::string convertedInt(stringStream.str());
	return (convertedInt.c_str());
}

FString UStaticFunctions::IntIPAddressToString(uint32 intAddress)
{
	uint32 ipAddressA = intAddress / (256 * 256 * 256);
	uint32 ipAddressB = (intAddress - ipAddressA * 256 * 256 * 256) / (256 * 256);
	uint32 ipAddressC = (intAddress - ipAddressA * 256 * 256 * 256 - ipAddressB * 256 * 256) / 256;
	uint32 ipAddressD = (intAddress - ipAddressA * 256 * 256 * 256 - ipAddressB * 256 * 256 - ipAddressC * 256);
	FString stringAddress = FString::FromInt(ipAddressA) + "." + FString::FromInt(ipAddressB) + "." + FString::FromInt(ipAddressC) + "." + FString::FromInt(ipAddressD);
	return stringAddress;
}

FString UStaticFunctions::SplitStringToLines(FString aString, int32 lineCharacterAmount)
{
	FString builtString = "";
	TArray<FString>stringArray = {};
	aString.ParseIntoArray(stringArray, TEXT(" "), false);
	int32 currentCharacterAmount = 0;
	for (int k = 0; k < stringArray.Num(); k++)
	{
		//builtString += stringArray[k] + " ";
		int32 newLineCharacterAmount = currentCharacterAmount + stringArray[k].Len() + 1;
		/*if (k != 0 && k % wordsPerLine == 0 || stringArray[k].Len() > lineBreakCharAmt)
		{
			builtString += "\n";
		}*/
		if (newLineCharacterAmount >= lineCharacterAmount)
		{
			builtString += "\n" + stringArray[k] + " ";
			currentCharacterAmount = stringArray[k].Len() + 1;
		}
		else {
			builtString += stringArray[k] + " ";
			currentCharacterAmount += stringArray[k].Len() + 1;
		}

	}
	return builtString;
}

//////////////////////////////////////////////////////////////////////////
//AbilityData
void UStaticFunctions::GenerateAbilityData(EPlayerAbilities abilityEnum, FString name, FString description, float cooldown, FString texturePath)
{
	FAbilityData abilityStruct = FAbilityData();
	abilityStruct.Name = name;
	abilityStruct.Description = description;
	abilityStruct.Cooldown = cooldown;
	abilityStruct.TexturePath = texturePath;
	m_AbilityMap.Add(abilityEnum, abilityStruct);
}

//////////////////////////////////////////////////////////////////////////
//TargetData
void UStaticFunctions::GenerateTargetData(ETargetTypes targetType, FString targetName, const FString& iconPath)
{
	FTargetData targetStruct = FTargetData();
	targetStruct.TargetName = targetName;
	targetStruct.IconPath = iconPath;
	m_TargetDataMap.Add(targetType, targetStruct);
}

//////////////////////////////////////////////////////////////////////////
//ItemDetails
void UStaticFunctions::GenerateItemDetails(FString itemName, FString itemDescription, EItemType itemType, const FString& texturePath)
{
	int itemID = m_ItemMap.Num();
	FItemDetails detailStruct = FItemDetails();
	detailStruct.ItemName = itemName;
	detailStruct.ItemDescription = itemDescription;
	detailStruct.ItemType = itemType;
	detailStruct.ItemTexturePath = texturePath;
	m_ItemMap.Add(itemID, detailStruct);
}

//////////////////////////////////////////////////////////////////////////
//ConsumableDetails
void UStaticFunctions::GenerateConsumableDetails(FString itemName, FString itemDescription, EConsumeType consumeType, int32 pointAmount, float cooldownTime, const FString& texturePath)
{
	int itemID = m_ItemMap.Num();
	FItemDetails detailStruct = FItemDetails();
	detailStruct.ItemName = itemName;
	detailStruct.ItemDescription = itemDescription;
	detailStruct.ItemType = EItemType::Consumable;
	detailStruct.ItemTexturePath = texturePath;
	m_ItemMap.Add(itemID, detailStruct);
	FConsumableData consumeStruct = FConsumableData();
	consumeStruct.ConsumeType = consumeType;
	consumeStruct.PointAmount = pointAmount;
	consumeStruct.CooldownTime = cooldownTime;
	m_ConsumableDataMap.Add(itemID, consumeStruct);
}

//////////////////////////////////////////////////////////////////////////
// GenerateWeaponDetails
void UStaticFunctions::GenerateWeaponDetails(FString itemName, EEquipmentSlot itemSlot, FString itemDescription, int32 attackPoints, float attackSpeed, FName socketName, FVector offset, FVector rotation, FVector scale, const FString& texturePath, const FString& classPath)
{
	int itemID = m_ItemMap.Num();
	FItemDetails detailStruct = FItemDetails();
	detailStruct.ItemName = itemName;
	detailStruct.ItemDescription = itemDescription;
	detailStruct.ItemType = EItemType::Weapon;
	detailStruct.ItemTexturePath = texturePath;
	m_ItemMap.Add(itemID, detailStruct);
	m_EquipmentSlotMap.Add(itemID, itemSlot);
	m_EquipmentClassMap.Add(itemID, classPath);
	FWeaponStats statStruct = FWeaponStats();
	statStruct.AttackPoints = attackPoints;
	statStruct.AttackSpeed = attackSpeed;
	m_WeaponStatsMap.Add(itemID, statStruct);
	FWornEquipmentData dataStruct = FWornEquipmentData();
	dataStruct.SocketName = socketName;
	dataStruct.Offset = offset;
	dataStruct.Rotation = rotation;
	dataStruct.Scale = scale;
	m_WornEquipmentDataMap.Add(itemID, dataStruct);
}

//////////////////////////////////////////////////////////////////////////
// GenerateArmorDetails
void UStaticFunctions::GenerateArmorDetails(FString itemName, EEquipmentSlot itemSlot, FString itemDescription, int32 attackPoints, int32 defensePoints, int32 healthPoints, int32 energyPoints, FName socketName, FVector offset, FVector rotation, FVector scale, const FString& texturePath, const FString& classPath)
{
	int itemID = m_ItemMap.Num();
	FItemDetails detailStruct = FItemDetails();
	detailStruct.ItemName = itemName;
	detailStruct.ItemDescription = itemDescription;
	detailStruct.ItemType = EItemType::Armor;
	detailStruct.ItemTexturePath = texturePath;
	m_ItemMap.Add(itemID, detailStruct);
	m_EquipmentSlotMap.Add(itemID, itemSlot);
	m_EquipmentClassMap.Add(itemID, classPath);
	FArmorStats statStruct = FArmorStats();
	statStruct.AttackPoints = attackPoints;
	statStruct.DefensePoints = defensePoints;
	statStruct.HealthPoints = healthPoints;
	statStruct.EnergyPoints = energyPoints;
	m_ArmorStatsMap.Add(itemID, statStruct);
	FWornEquipmentData dataStruct = FWornEquipmentData();
	dataStruct.SocketName = socketName;
	dataStruct.Offset = offset;
	dataStruct.Rotation = rotation;
	dataStruct.Scale = scale;
	m_WornEquipmentDataMap.Add(itemID, dataStruct);
}

void UStaticFunctions::GenerateSlayingQuestData(FString questName, FString questDescription, int32 requiredKills, FString enemyFaction, int32 xpReward, TArray<FString> itemRewards)
{
	FQuestDetails questDetails = FQuestDetails();
	questDetails.Name = questName;
	questDetails.Description = questDescription;
	questDetails.Type = EQuestTypes::Slaying;
	questDetails.ItemRetrievalProgress = FItemRetrievalQuestDetails();
	FSlayingQuestDetails slayingDetails = FSlayingQuestDetails();
	slayingDetails.RequiredKills = requiredKills;
	slayingDetails.FactionName = enemyFaction;
	questDetails.SlayingProgress = slayingDetails;
	questDetails.XPReward = xpReward;
	questDetails.ItemRewards = itemRewards;
	m_QuestMap.Add(questName, questDetails);
}

void UStaticFunctions::GenerateItemRetrievalQuestData(FString questName, FString questDescription, int32 requiredItems, FString itemName, int32 xpReward, TArray<FString> itemRewards)
{
	FQuestDetails questDetails = FQuestDetails();
	questDetails.Name = questName;
	questDetails.Description = questDescription;
	questDetails.Type = EQuestTypes::ItemRetrieval;
	questDetails.SlayingProgress = FSlayingQuestDetails();
	FItemRetrievalQuestDetails retrievalDetails = FItemRetrievalQuestDetails();
	retrievalDetails.RequiredItems = requiredItems;
	retrievalDetails.ItemName = itemName;
	questDetails.ItemRetrievalProgress = retrievalDetails;
	questDetails.XPReward = xpReward;
	questDetails.ItemRewards = itemRewards;
	m_QuestMap.Add(questName, questDetails);
}

//////////////////////////////////////////////////////////////////////////
// GetItemIDByName
int32 UStaticFunctions::GetItemIDByName(FString itemName)
{
	int32 id = -1;
	for (auto It = m_ItemMap.CreateConstIterator(); It; ++It)
	{
		FItemDetails itemStruct = It.Value();
		if (itemStruct.ItemName == itemName)
		{
			id = It.Key();
			break;
		}
	}
	return id;
}

//////////////////////////////////////////////////////////////////////////
// ItemIDExists
bool UStaticFunctions::ItemIDExists(int32 itemID)
{
	if (itemID == -1) return false;
	return (m_ItemMap.Contains(itemID));
}

EEquipmentSlot UStaticFunctions::GetEquipmentSlotByID(int32 itemID)
{
	if (m_EquipmentSlotMap.Contains(itemID))
		return ((EEquipmentSlot)m_EquipmentSlotMap[itemID]);
	return EEquipmentSlot::Back;
}

//////////////////////////////////////////////////////////////////////////
// SetEquipmentSlot
void UStaticFunctions::SetEquipmentSlot(AActor* theActor, EEquipmentSlot equipSlot, AActor* theItemActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer) return;
	AWornEquipmentActorParent* theEquipment = NULL;
	if (theItemActor)
		theEquipment = Cast<AWornEquipmentActorParent>(theItemActor);

	switch (equipSlot)
	{
		case EEquipmentSlot::Head:
			thePlayer->m_Equipment.HeadSlot = theEquipment;
		break;
		case EEquipmentSlot::LeftShoulder:
			thePlayer->m_Equipment.LeftShoulderSlot = theEquipment;
		break;
		case EEquipmentSlot::RightShoulder:
			thePlayer->m_Equipment.RightShoulderSlot = theEquipment;
		break;
		case EEquipmentSlot::Chest:
			thePlayer->m_Equipment.ChestSlot = theEquipment;
		break;
		case EEquipmentSlot::Back:
			thePlayer->m_Equipment.BackSlot = theEquipment;
		break;
		case EEquipmentSlot::LeftHand:
			thePlayer->m_Equipment.LeftHandSlot = theEquipment;
		break;
		case EEquipmentSlot::RightHand:
			thePlayer->m_Equipment.RightHandSlot = theEquipment;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
// GetEquipmentSlot
AWornEquipmentActorParent* UStaticFunctions::GetEquipmentSlot(AActor* theActor, EEquipmentSlot equipSlot)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer) return NULL;
	switch (equipSlot)
	{
		case EEquipmentSlot::Head:
			return(thePlayer->m_Equipment.HeadSlot);
		break;
		case EEquipmentSlot::LeftShoulder:
			return(thePlayer->m_Equipment.LeftShoulderSlot);
		break;
		case EEquipmentSlot::RightShoulder:
			return(thePlayer->m_Equipment.RightShoulderSlot);
		break;
		case EEquipmentSlot::Chest:
			return(thePlayer->m_Equipment.ChestSlot);
		break;
		case EEquipmentSlot::Back:
			return(thePlayer->m_Equipment.BackSlot);
		break;
		case EEquipmentSlot::LeftHand:
			return(thePlayer->m_Equipment.LeftHandSlot);
		break;
		case EEquipmentSlot::RightHand:
			return(thePlayer->m_Equipment.RightHandSlot);
		break;
	}

	return NULL;
}

UClass* UStaticFunctions::GetEquipmentClassByName(UWorld* theWorld, FString itemName)
{
	ARPGDemoGameMode* theGamemode = Cast<ARPGDemoGameMode>(theWorld->GetAuthGameMode());
	if (theGamemode) {
		for (FEquipmentBlueprint equipStruct : theGamemode->m_EquipmentBlueprints)
		{
			if (equipStruct.ItemName == itemName)
			{
				return equipStruct.ItemActor;
			}
				
		}
	}
	return NULL;
}

EPlayerAbilities UStaticFunctions::IndexToActionSlot(int index, AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer) return EPlayerAbilities::None;
	switch (index)
	{
		case 0:
			return thePlayer->m_ActionKeys.ActionKeyOne;
		break;
		case 1:
			return thePlayer->m_ActionKeys.ActionKeyTwo;
		break;
		case 2:
			return thePlayer->m_ActionKeys.ActionKeyThree;
		break;
		case 3:
			return thePlayer->m_ActionKeys.ActionKeyFour;
		break;
		case 4:
			return thePlayer->m_ActionKeys.ActionKeyFive;
		break;
		case 5:
			return thePlayer->m_ActionKeys.ActionKeySix;
		break;
		case 6:
			return thePlayer->m_ActionKeys.ActionKeySeven;
		break;
		case 7:
			return thePlayer->m_ActionKeys.ActionKeyEight;
		break;
		case 8:
			return thePlayer->m_ActionKeys.ActionKeyNine;
		break;
	}
	return EPlayerAbilities::None;
}

//////////////////////////////////////////////////////////////////////////
// DebugMessage
void UStaticFunctions::DebugMessage(FColor displayColor, const FString& message)
{
	if (!GEngine) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, displayColor, message);
}

//////////////////////////////////////////////////////////////////////////
// LoadTextureFromPath
UTexture2D* UStaticFunctions::LoadTextureFromPath(const FString& aPath)
{
	UTexture2D* aTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(aPath)));
	return aTexture;
}

//////////////////////////////////////////////////////////////////////////
// LoadMaterialFromPath
UMaterial* UStaticFunctions::LoadMaterialFromPath(const FString& aPath)
{
	UMaterial* aMaterial = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), NULL, *(aPath)));
	return aMaterial;
}

//////////////////////////////////////////////////////////////////////////
// LoadFontFromPath
UFont* UStaticFunctions::LoadFontFromPath(const FString& aPath)
{
	UFont* aFont = Cast<UFont>(StaticLoadObject(UFont::StaticClass(), NULL, *(aPath)));
	return aFont;
}

//////////////////////////////////////////////////////////////////////////
// LoadStaticMeshFromPath
UStaticMesh* UStaticFunctions::LoadStaticMeshFromPath(const FString& aPath)
{
	UStaticMesh* aMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), NULL, *(aPath)));
	return aMesh;
}

//////////////////////////////////////////////////////////////////////////
// LoadParticleSystemFromPath
UParticleSystem* UStaticFunctions::LoadParticleSystemFromPath(const FString& aPath)
{
	UParticleSystem* aParticleSystem = Cast<UParticleSystem>(StaticLoadObject(UParticleSystem::StaticClass(), NULL, *(aPath)));
	return aParticleSystem;
}

//////////////////////////////////////////////////////////////////////////
// LoadSoundCueFromPath
USoundCue* UStaticFunctions::LoadSoundCueFromPath(const FString& aPath)
{
	USoundCue* aSoundCue = Cast<USoundCue>(StaticLoadObject(USoundCue::StaticClass(), NULL, *(aPath)));
	return aSoundCue;
}

bool UStaticFunctions::ActorIsFacingLocation(AActor* theActor, FVector theLocation)
{
	if (!theActor) return false;
	FVector normalVector = theLocation - theActor->GetActorLocation();
	normalVector.Normalize();
	return (FVector::DotProduct(theActor->GetActorForwardVector(), normalVector) > 0.6);
}

FLinearColor UStaticFunctions::RainbowStrobe(UWorld* theWorld, float frequency, float alpha)
{
	float currentTime = theWorld->GetRealTimeSeconds();
	float red = (FMath::Sin(frequency * currentTime) * 127 + 128) / 255;
	float green = (FMath::Sin(frequency * currentTime + 2) * 127 + 128) / 255;
	float blue = (FMath::Sin(frequency * currentTime + 4) * 127 + 128) / 255;
	return (FLinearColor(red, green, blue, alpha));
}

void UStaticFunctions::ApplyForce(AActor* theActor, FVector dir, float force)
{
	ACharacter* theCharacter = Cast<ACharacter>(theActor);
	if (!theCharacter)
		return;

	theCharacter->GetCharacterMovement()->AddImpulse(dir * force, true);
}

void UStaticFunctions::SetParticleEmitterParameters(UParticleSystemComponent* particleSystem, TArray<FParticleParameter> parameterArray)
{
	for (FParticleParameter particleParameter : parameterArray)
	{
		switch (particleParameter.ParameterType)
		{
		case EParticleParameterTypes::Scalar:
			particleSystem->SetFloatParameter(particleParameter.ParameterName, particleParameter.ScalarValue);
			break;
		case EParticleParameterTypes::Vector:
			particleSystem->SetVectorParameter(particleParameter.ParameterName, particleParameter.VectorValue);
			break;
		case EParticleParameterTypes::Color:
			particleSystem->SetColorParameter(particleParameter.ParameterName, particleParameter.ColorValue);
			break;
		case EParticleParameterTypes::Actor:
			particleSystem->SetActorParameter(particleParameter.ParameterName, particleParameter.ActorValue);
			break;
		case EParticleParameterTypes::Material:
			particleSystem->SetMaterialParameter(particleParameter.ParameterName, particleParameter.MaterialValue);
			break;
		}
	}
}

void UStaticFunctions::SetObjectForDestruction(UWorld* theWorld, UObject* theObject, float length)
{
	if (!theObject || !theWorld) return;
	AActor* anActor = Cast<AActor>(theObject);
	UActorComponent* aComponent = Cast<UActorComponent>(theObject);
	FTimerDelegate objectTimerDelegate;
	objectTimerDelegate.BindLambda([=]{
		if (anActor)
			anActor->Destroy();
		else if (aComponent)
			aComponent->DestroyComponent();
	});
	FTimerHandle objectTimerHandle;
	theWorld->GetTimerManager().SetTimer(objectTimerHandle, objectTimerDelegate, length, false);
}

void UStaticFunctions::TriggerDelayedAction(UWorld* theWorld, FTimerDelegate timerDelegate, float length, bool shouldLoop)
{
	if (!theWorld || !timerDelegate.IsBound()) return;
	FTimerHandle actionTimerHandle;
	theWorld->GetTimerManager().SetTimer(actionTimerHandle, timerDelegate, length, shouldLoop);
}

float UStaticFunctions::GetActorHeight(AActor* theActor)
{
	float actorSize = 0;
	UActorComponent* staticMesh = theActor->GetComponentByClass(UStaticMeshComponent::StaticClass());
	UActorComponent* skeletalMesh = theActor->GetComponentByClass(USkeletalMeshComponent::StaticClass());
	UStaticMeshComponent* staticMeshComponent = Cast<UStaticMeshComponent>(staticMesh);
	USkeletalMeshComponent* skeletalMeshComponent = Cast<USkeletalMeshComponent>(skeletalMesh);
	if (staticMesh && staticMeshComponent)
		actorSize = staticMeshComponent->Bounds.BoxExtent.Z;
	else if (skeletalMesh && skeletalMeshComponent)
		actorSize = skeletalMeshComponent->Bounds.BoxExtent.Z;

	return actorSize;
}

FVector UStaticFunctions::GetActorCollisionBounds(AActor* theActor)
{
	FVector collisionBounds = FVector(0, 0, 0);
	UActorComponent* boxComponent = theActor->GetComponentByClass(UBoxComponent::StaticClass());
	UActorComponent* sphereComponent = theActor->GetComponentByClass(USphereComponent::StaticClass());
	UActorComponent* capsuleComponent = theActor->GetComponentByClass(UCapsuleComponent::StaticClass());
	UBoxComponent* _boxComponent = Cast<UBoxComponent>(boxComponent);
	USphereComponent* _sphereComponent = Cast<USphereComponent>(sphereComponent);
	UCapsuleComponent* _capsuleComponent = Cast<UCapsuleComponent>(capsuleComponent);
	if (boxComponent && _boxComponent)
	{
		FVector boxExtent = _boxComponent->GetUnscaledBoxExtent();
		collisionBounds = FVector(boxExtent.X, boxExtent.Y, boxExtent.Z);
	}
	else if (sphereComponent && _sphereComponent)
	{
		float radius = _sphereComponent->GetUnscaledSphereRadius();
		collisionBounds = FVector(radius, radius, radius);
	}
	else if (capsuleComponent && _capsuleComponent)
	{
		float radius = _capsuleComponent->GetUnscaledCapsuleRadius();
		float halfHeight = _capsuleComponent->GetUnscaledCapsuleHalfHeight();
		collisionBounds = FVector(radius, radius, halfHeight);
	}

	return collisionBounds;
}

FPlayerUMGPosition UStaticFunctions::GetDefinedUMGPosition(UClass* widgetClass, TArray<FPlayerUMGPosition> positionArray)
{
	FPlayerUMGPosition defaultPosition = FPlayerUMGPosition();
	for (FPlayerUMGPosition positionData : positionArray)
	{
		if (widgetClass == positionData.WidgetClass)
		{
			return positionData;
		}
	}
	return defaultPosition;
}

FViewportData UStaticFunctions::GetViewportData()
{
	FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	float viewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));
	FViewportData viewportData = FViewportData();
	viewportData.ViewportSize = viewportSize;
	viewportData.ViewportScale = viewportScale;
	return viewportData;
}