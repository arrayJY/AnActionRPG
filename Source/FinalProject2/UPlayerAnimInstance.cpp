// Fill out your copyright notice in the Description page of Project Settings.


#include "UPlayerAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	IsAttacking = false;
	IsBlocking = false;
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
		FOnMontageEnded EndedDelegate;
		EndedDelegate.BindUObject(this, &UPlayerAnimInstance::OnAttackEnd);
		Montage_SetEndDelegate(EndedDelegate, AttackMontage);
	}
}

void UPlayerAnimInstance::PlayDamagedAnimation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Damaged1"));
	if(DamagedMontage)
	{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Damaged2"));
		Montage_Play(DamagedMontage);
	}
}

void UPlayerAnimInstance::OnAttackEnd(UAnimMontage* animMontage, bool bInterrupted)
{
	IsAttacking1 = false;
}
