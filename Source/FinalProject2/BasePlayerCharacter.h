// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "UPlayerAnimInstance.h"
#include "WithWeaponSceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent)
	void MoveForward(float Value);
	UFUNCTION(BlueprintCallable)
	void MoveForwardCommon(float Value);

	UFUNCTION(BlueprintNativeEvent)
	void MoveRight(float Value);
	UFUNCTION(BlueprintCallable)
	void MoveRightCommon(float Value);

	UFUNCTION()
	void Roll();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartAttack();
	UFUNCTION(BlueprintCallable)
	void StartAttackCommon();

	UFUNCTION()
	void EndAttack();

	UFUNCTION()
	void StartBlock();

	UFUNCTION()
	void EndBlock();

	UFUNCTION()
	void YawCamera(float Val);

	UFUNCTION()
	void PitchCamera(float Val);

	UFUNCTION()
	void SetMeshRotation(float Val);
	UFUNCTION()
	void ControllerRotateToCameraDirection();

	UPROPERTY(EditAnywhere)
	float Health;
	UFUNCTION()
	void DecreaseHealth(float Val);
	UFUNCTION(BlueprintImplementableEvent, Category="Player")
	void SetHealthBar(float Val);
	UFUNCTION(BlueprintImplementableEvent, Category="Player")
	void SetScoreText(int Val);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Player")
	bool IsDamaged;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPlayerAnimInstance* Animation;

	UPROPERTY(VisibleAnywhere)
	ABasePlayerCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATKValue;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DEFRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score;
	
	UFUNCTION()
	void IncreaseScore(float ATK, float DEF);
};
