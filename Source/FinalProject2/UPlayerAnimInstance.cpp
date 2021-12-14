// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "UPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	IsAttacking = false;
	IsBlocking = true;
	IsFalling = false;
	IsRunning = false;
	UpperBodyBlendAlpha = 1.0;
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	ACharacter* Character = Cast<ACharacter>(TryGetPawnOwner());
	if (Character)
	{
		auto Movement = Character->GetCharacterMovement();
		IsRunning = Movement->GetLastUpdateVelocity().Size() > 0.0;
		IsFalling = Movement->MovementMode == MOVE_Falling;
	}
}

void UPlayerAnimInstance::PlayAttackAnimation()
{
	if (AttackMontage && !IsAttacking1)
	{
		Montage_Play(AttackMontage);
		IsAttacking1 = true;
		FOnMontageEnded AttackEndedDelegate;
		AttackEndedDelegate.BindUObject(this, &UPlayerAnimInstance::OnAttackEnd);
		Montage_SetEndDelegate(AttackEndedDelegate, AttackMontage);
	}
}

void UPlayerAnimInstance::OnDamagedEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	auto Character = Cast<ABasePlayerCharacter>(TryGetPawnOwner());
	Character->IsDamaged = false;
}


void UPlayerAnimInstance::PlayDamagedAnimation()
{
	if (DamagedMontage)
	{
		Montage_Stop(0.25, AttackMontage);
		Montage_Play(DamagedMontage);
		FOnMontageEnded DamagedEndedDelegate;
		DamagedEndedDelegate.BindUObject(this, &UPlayerAnimInstance::OnDamagedEnd);
		Montage_SetEndDelegate(DamagedEndedDelegate, DamagedMontage);
	}
}

void UPlayerAnimInstance::PlayDieAnimation()
{
	if (DieMontage)
	{
		Montage_Stop(0.25);
		Montage_Play(DieMontage);
	}
}


void UPlayerAnimInstance::OnAttackEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	IsAttacking1 = false;
}
