// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "WornEquipmentActorParent.h"
#include "UnrealNetwork.h"

AWornEquipmentActorParent::AWornEquipmentActorParent(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	SetRemoteRoleForBackwardsCompat(ROLE_SimulatedProxy);
	bReplicates = true;
	//bReplicateMovement = true;
	PrimaryActorTick.bCanEverTick = true;
	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	m_StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_StaticMesh->SetIsReplicated(true);
	RootComponent = m_StaticMesh;
}

//////////////////////////////////////////////////////////////////////////
// GetLifetimeReplicatedProps
void AWornEquipmentActorParent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWornEquipmentActorParent, m_ItemName);
}

void AWornEquipmentActorParent::BeginPlay()
{
	Super::BeginPlay();
}

void AWornEquipmentActorParent::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (RootComponent->GetAttachParent() && m_Debug) {
		USceneComponent* attachParent = RootComponent->GetAttachParent();
		RootComponent->DetachFromParent();
		FVector socketLocation = attachParent->GetSocketLocation(m_SocketName);
		SetActorRelativeRotation(attachParent->GetSocketRotation(m_SocketName));
		AddActorLocalRotation(FQuat::MakeFromEuler(m_Rotation));
		SetActorRelativeLocation(socketLocation);
		AddActorLocalOffset(m_Offset);
		GetRootComponent()->SetWorldScale3D(m_Scale);
		GetRootComponent()->AttachTo(attachParent, m_SocketName, EAttachLocation::KeepWorldPosition);
	}
}

