// Fill out your copyright notice in the Description page of Project Settings.


#include "UPlayerAnimInstance.h"
#include "BasePlayerCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	IsAttacking = false;
	IsBlocking = false;
	IsRolling = false;
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
		IsFalling = Movement->MovementMode == EMovementMode::MOVE_Falling;
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
		// Montage_Stop(0.25);
		Montage_Play(DamagedMontage);
		FOnMontageEnded DamagedEndedDelegate;
		DamagedEndedDelegate.BindUObject(this, &UPlayerAnimInstance::OnDamagedEnd);
		Montage_SetEndDelegate(DamagedEndedDelegate, DamagedMontage);
	} else{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No Montage"));    
	}
}

void UPlayerAnimInstance::PlayRollAnimation()
{
	if (RollMontage)
	{
		IsRolling = true;
		Montage_Stop(0.1, AttackMontage);
		Montage_Stop(0.1, BlockReactMontage);
		Montage_Play(RollMontage, 1.5);
		FOnMontageEnded RollEndedDelegate;
		FOnMontageBlendingOutStarted BlendOutDelegate;
		RollEndedDelegate.BindUObject(this, &UPlayerAnimInstance::OnRollEnd);
		BlendOutDelegate.BindUObject(this, &UPlayerAnimInstance::OnRollBlendOut);
		Montage_SetEndDelegate(RollEndedDelegate);
		Montage_SetBlendingOutDelegate(BlendOutDelegate);
	}
}

void UPlayerAnimInstance::OnRollEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	IsRolling = false;
	Cast<ABasePlayerCharacter>(TryGetPawnOwner())->Speed = 1.0;
}

void UPlayerAnimInstance::OnRollBlendOut(UAnimMontage* animMontage, bool bInterrupted)
{
	IsRolling = false;
	Cast<ABasePlayerCharacter>(TryGetPawnOwner())->Speed = 1.0;
}

void UPlayerAnimInstance::PlayDieAnimation()
{
	if (DieMontage)
	{
		Montage_Stop(0.25);
		Montage_Play(DieMontage, 1.0);
		FOnMontageBlendingOutStarted BlendOutDelegate;
		BlendOutDelegate.BindLambda([&](UAnimMontage* animMontage, bool bInterrupted)
		{
			Montage_SetPlayRate(DieMontage, 0.0);
			TryGetPawnOwner()->Destroy();
		});
		Montage_SetBlendingOutDelegate(BlendOutDelegate);
	}
}

void UPlayerAnimInstance::PlayBlockReactAnimation()
{
	if (BlockReactMontage)
	{
		Montage_Stop(0.1, BlockReactMontage);
		Montage_Play(BlockReactMontage, 2.0, EMontagePlayReturnType::MontageLength, 0.25);
	}
}


void UPlayerAnimInstance::OnAttackEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	IsAttacking1 = false;
}
