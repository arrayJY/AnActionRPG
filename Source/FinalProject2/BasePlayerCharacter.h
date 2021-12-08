// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "BasePlayerCharacter.generated.h"


UCLASS()
class FINALPROJECT2_API ABasePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasePlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 处理用于前后移动的输入。
	UFUNCTION()
	void MoveForward(float Value);

	// 处理用于左右移动的输入。
	UFUNCTION()
	void MoveRight(float Value);

	// 按下键时，设置跳跃标记。
	UFUNCTION()
	void StartJump();

	// 释放键时，清除跳跃标记。
	UFUNCTION()
	void StopJump();

	// FPS摄像机
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* FPSCameraComponent;
};
