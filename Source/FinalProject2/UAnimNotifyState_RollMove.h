// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BasePlayerCharacter.h"
#include "UAnimNotifyState_RollMove.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class FINALPROJECT2_API UAnimNotifyState_RollMove : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	ABasePlayerCharacter* Character;
	
	UPROPERTY(VisibleAnywhere)
	FVector Direction;
	
	UAnimNotifyState_RollMove();
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
