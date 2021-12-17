// Fill out your copyright notice in the Description page of Project Settings.


#include "UAnimNotifyState_RollMove.h"

UAnimNotifyState_RollMove::UAnimNotifyState_RollMove()
{
	Character = CreateDefaultSubobject<ABasePlayerCharacter>("Character");
	Direction = FVector(0, 0, 0);
}

void UAnimNotifyState_RollMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration)
{
	UAnimNotifyState::NotifyBegin(MeshComp, Animation, TotalDuration);
	Character = Cast<ABasePlayerCharacter>(MeshComp->GetOwner());
	Direction = MeshComp->GetForwardVector().RotateAngleAxis(90, FVector(0, 0, 1));
	if (Character)
	{
		Character->Animation->IsRolling = true;
	}
}

void UAnimNotifyState_RollMove::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float FrameDeltaTime)
{
	UAnimNotifyState::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	if (Character)
	{
		Character->AddMovementInput(Direction, Character->Speed * 8);
	}
}

void UAnimNotifyState_RollMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UAnimNotifyState::NotifyEnd(MeshComp, Animation);
	if (Character)
	{
		Character->Animation->IsRolling = false;
	}
}
