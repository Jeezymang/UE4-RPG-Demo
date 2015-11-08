// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGDemo.h"
#include "StaticPlayerAbilities.h"
#include "RPGDemoCharacter.h"
#include "Bots/BotCharacterParent.h"

void UStaticPlayerAbilities::AttemptHeavyAttack(AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer || !thePlayer->m_Target)
		return;
	thePlayer->ClientPlaySoundAtLocation(UStaticFunctions::LoadSoundCueFromPath("/Game/Sounds/SC_Swoosh02"), thePlayer->GetActorLocation());
	thePlayer->ClientStopAllAnimMontages();
	thePlayer->ClientPlayAnimMontage(thePlayer->m_AnimMontages.HeavyAttack);
	InflictDamageOnTarget(theActor, thePlayer->GetDamageBoost(FMath::RandRange(5, 15)));
}

void UStaticPlayerAbilities::AttemptKnockout(AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer || !thePlayer->m_Target)
		return;
	ARPGDemoCharacter* theTargetPlayer = Cast<ARPGDemoCharacter>(thePlayer->m_Target);
	ABotCharacterParent* theBot = Cast<ABotCharacterParent>(thePlayer->m_Target);
	thePlayer->ClientStopAllAnimMontages();
	thePlayer->ClientPlayAnimMontage(thePlayer->m_AnimMontages.Knockout);

	FTimerDelegate actionDelegate;
	actionDelegate.BindLambda([=] {
		if (!thePlayer) return;

		if (theTargetPlayer)
			theTargetPlayer->TriggerStun(5);
		else if (theBot)
			theBot->TriggerStun(5);
		TArray<FParticleParameter> parameterArray;
		thePlayer->ClientPlaySoundAtLocation(UStaticFunctions::LoadSoundCueFromPath("/Game/Sounds/SC_Smash01"), thePlayer->GetActorLocation());
		thePlayer->ClientSpawnParticleEmitterAtLocation("/Game/ParticleSystems/PS_Stunned", thePlayer->m_Target->GetActorLocation() + FVector(0, 0, UStaticFunctions::GetActorCollisionBounds(thePlayer->m_Target).Z + 5), FRotator(0, 0, 0), parameterArray, 5);
	});
	UStaticFunctions::TriggerDelayedAction(thePlayer->GetWorld(), actionDelegate, 0.3);
}

void UStaticPlayerAbilities::AttemptWeakPushback(AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer || !thePlayer->m_Target)
		return;

	FTimerDelegate actionDelegate;
	actionDelegate.BindLambda([=] {
		if (!thePlayer || thePlayer->m_IsDead || thePlayer->m_IsStunned) return;
		thePlayer->ClientPlaySoundAtLocation(UStaticFunctions::LoadSoundCueFromPath("/Game/Sounds/SC_Swoosh01"), thePlayer->GetActorLocation());
		FVector particleVelocity = thePlayer->m_Target->GetActorLocation() - thePlayer->GetActorLocation();
		particleVelocity.Normalize();
		particleVelocity = particleVelocity * 500;
		FParticleParameter particleParameter = FParticleParameter();
		particleParameter.ParameterName = "ParticleVelocity";
		particleParameter.ParameterType = EParticleParameterTypes::Vector;
		particleParameter.VectorValue = particleVelocity;
		TArray<FParticleParameter> parameterArray;
		parameterArray.Add(particleParameter);
		thePlayer->ClientSpawnParticleEmitterAtLocation("/Game/ParticleSystems/PS_Knockback", thePlayer->GetActorLocation(), FRotator(0, 0, 0), parameterArray);
		UStaticFunctions::ApplyForce(thePlayer->m_Target, thePlayer->GetActorForwardVector() + thePlayer->GetActorUpVector(), 500.0);
		InflictDamageOnTarget(theActor, thePlayer->GetDamageBoost(FMath::FRandRange(5, 10)));
	});
	UStaticFunctions::TriggerDelayedAction(theActor->GetWorld(), actionDelegate, 0.4);
	thePlayer->ClientStopAllAnimMontages();
	thePlayer->ClientPlayAnimMontage(thePlayer->m_AnimMontages.Pushback);
}

void UStaticPlayerAbilities::AttemptWeakLifeDrain(AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer || !thePlayer->m_Target)
		return;
	thePlayer->ClientPlaySoundAtLocation(UStaticFunctions::LoadSoundCueFromPath("/Game/Sounds/SC_Swish01"), thePlayer->GetActorLocation());
	FVector particleVelocity = thePlayer->m_Target->GetActorLocation() - thePlayer->GetActorLocation();
	particleVelocity.Normalize();
	particleVelocity = particleVelocity * 200;
	FParticleParameter particleParameter = FParticleParameter();
	particleParameter.ParameterName = "ParticleVelocity";
	particleParameter.ParameterType = EParticleParameterTypes::Vector;
	particleParameter.VectorValue = particleVelocity;
	TArray<FParticleParameter> parameterArray;
	parameterArray.Add(particleParameter);
	thePlayer->ClientSpawnParticleEmitterAttached("/Game/ParticleSystems/PS_LifeDrain", FName("Chest_Socket"), FVector(0, 0, 0), FRotator(0, 0, 0), parameterArray);
	thePlayer->ClientStopAllAnimMontages();
	thePlayer->ClientPlayAnimMontage(thePlayer->m_AnimMontages.DrainLife);
	int32 drainAmount = thePlayer->GetDamageBoost(FMath::RandRange(5, 15));
	InflictDamageOnTarget(theActor, drainAmount);
	thePlayer->m_Health = FMath::Clamp(thePlayer->m_Health + drainAmount, 0, thePlayer->m_MaxHealth);
}

void UStaticPlayerAbilities::AttemptStrikedown(AActor* theActor)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer || !thePlayer->m_Target)
		return;
	thePlayer->ClientStopAllAnimMontages();
	thePlayer->ClientPlayAnimMontage(thePlayer->m_AnimMontages.Strikedown);
	FTimerDelegate actionDelegate;
	thePlayer->ClientPlaySoundAtLocation(UStaticFunctions::LoadSoundCueFromPath("/Game/Sounds/SC_Strikedown"), thePlayer->GetActorLocation());
	actionDelegate.BindLambda([=] {
		if (!thePlayer || thePlayer->m_IsDead || thePlayer->m_IsStunned) return;
		
		InflictDamageOnTarget(theActor, thePlayer->GetDamageBoost(FMath::RandRange(5, 20)));
	});
	UStaticFunctions::TriggerDelayedAction(theActor->GetWorld(), actionDelegate, 0.4);
}

void UStaticPlayerAbilities::InflictDamageOnTarget(AActor* theActor, int32 damageAmount)
{
	ARPGDemoCharacter* thePlayer = Cast<ARPGDemoCharacter>(theActor);
	if (!thePlayer || !thePlayer->m_Target)
		return;
	ARPGDemoCharacter* thePlayerTarget = Cast<ARPGDemoCharacter>(thePlayer->m_Target);
	ABotCharacterParent* theBotTarget = Cast<ABotCharacterParent>(thePlayer->m_Target);
	if (thePlayerTarget)
	{
		thePlayerTarget->InflictDamage(damageAmount, thePlayer);
	}
	else if (theBotTarget)
	{
		theBotTarget->InflictDamage(damageAmount, thePlayer);
	}
}
