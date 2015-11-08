// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Libraries/StaticEnums.h"
#include "WornEquipmentActorParent.generated.h"

UCLASS()
class RPGDEMO_API AWornEquipmentActorParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWornEquipmentActorParent(const FObjectInitializer& ObjectInitializer);

	UStaticMeshComponent* m_StaticMesh;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FString m_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FName m_SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FVector m_Offset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FVector m_Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	FVector m_Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
	bool m_Debug;

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;
};
