// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemPickupActorParent.generated.h"

UCLASS()
class RPGDEMO_API AItemPickupActorParent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickupActorParent(const FObjectInitializer& ObjectInitializer);

	USphereComponent* m_CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Item)
	FString m_ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Collision)
	float m_CollisionSphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	bool m_PulseScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	float m_PulseScaleAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	bool m_Rotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Effects)
	FVector m_RotateAxis;

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;
};
