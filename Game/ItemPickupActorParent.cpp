// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "ItemPickupActorParent.h"


// Sets default values
AItemPickupActorParent::AItemPickupActorParent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	bReplicates = true;
	m_CollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere Collider"));
	m_CollisionComponent->InitSphereRadius(60);
	RootComponent = m_CollisionComponent;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AItemPickupActorParent::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("ItemPickup");
	m_CollisionComponent->ComponentTags.Add("ItemPickup");
	m_CollisionComponent->SetSphereRadius(m_CollisionSphereRadius);
}

// Called every frame
void AItemPickupActorParent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (m_PulseScale)
	{
		float multi = FMath::Sin(GetWorld()->GetRealTimeSeconds() * 3) * (double)m_PulseScaleAmount;
		if (multi < 0)
			multi = multi * -1;
		multi += m_PulseScaleAmount * 0.5;
		SetActorRelativeScale3D(FVector(multi, multi, multi));
	}
	if (m_Rotate)
	{
		float angleMultiX = FMath::Sin(GetWorld()->GetRealTimeSeconds()*m_RotateAxis.X) * 360;
		float angleMultiY = FMath::Sin(GetWorld()->GetRealTimeSeconds()*m_RotateAxis.Y) * 360;
		float angleMultiZ = FMath::Sin(GetWorld()->GetRealTimeSeconds()*m_RotateAxis.Z) * 360;
		SetActorRelativeRotation(FQuat::MakeFromEuler(FVector(angleMultiX, angleMultiY, angleMultiZ)));
	}
}