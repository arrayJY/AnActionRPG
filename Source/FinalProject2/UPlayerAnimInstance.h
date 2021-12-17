// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class FINALPROJECT2_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPlayerAnimInstance();

	UFUNCTION()
	void PlayAttackAnimation();

	UFUNCTION()
	void PlayDamagedAnimation();

	UFUNCTION()
	void PlayDieAnimation();

	UFUNCTION()
	void PlayBlockReactAnimation();
	
	UFUNCTION()
	void PlayRollAnimation();
	
	UFUNCTION()
	void OnRollEnd(UAnimMontage* animMontage, bool bInterrupted);
	
	UFUNCTION()
	void OnRollBlendOut(UAnimMontage* animMontage, bool bInterrupted);

	UFUNCTION()
	void OnAttackEnd(UAnimMontage* animMontage, bool bInterrupted);

	UFUNCTION()
	void OnDamagedEnd(UAnimMontage* animMontage, bool bInterrupted);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerState)
	bool IsRunning;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerState)
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerState)
	bool IsRolling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerState)
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerState)
	bool IsAttacking1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerState)
	bool IsBlocking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerState)
	float UpperBodyBlendAlpha;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimationSequence)
	UAnimSequence* IdleAnimSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimationSequence)
	UAnimSequence* RunningAnimSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimationSequence)
	UAnimSequence* BlockingAnimSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimationSequence)
	UAnimSequence* FallingAnimSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimationSequence)
	UAnimSequence* CrouchAnimSequence;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=UpperBodyAnimation)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimation)
	UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimation)
	UAnimMontage* DieMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimation)
	UAnimMontage* BlockReactMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PlayerAnimation)
	UAnimMontage* RollMontage;
};
