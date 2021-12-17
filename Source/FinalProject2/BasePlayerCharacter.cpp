// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
	IsDamaged = false;
	Health = 100.0;
	Speed = 1.0;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 200.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 4.0f;
	SpringArmComp->CameraLagMaxDistance = 50.f;
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABasePlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	Animation = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void ABasePlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called to bind functionality to input
void ABasePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 设置"移动"绑定。
	PlayerInputComponent->BindAxis("MoveForward", this, &ABasePlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABasePlayerCharacter::MoveRight);

	// 设置"观看"绑定。
	InputComponent->BindAxis("CameraPitch", this, &ABasePlayerCharacter::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &ABasePlayerCharacter::YawCamera);

	// 设置"操作"绑定。
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ABasePlayerCharacter::Roll);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABasePlayerCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABasePlayerCharacter::EndAttack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ABasePlayerCharacter::StartBlock);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &ABasePlayerCharacter::EndBlock);
}


void ABasePlayerCharacter::SetMeshRotation(float Val)
{
	const float MeshInitDegree = -90.0;
	FRotator MeshRotation = GetMesh()->GetComponentRotation();
	MeshRotation.Yaw = SpringArmComp->GetComponentRotation().Yaw + MeshInitDegree;
	MeshRotation.Yaw += Val;
	GetMesh()->SetWorldRotation(MeshRotation);
}

void ABasePlayerCharacter::ControllerRotateToCameraDirection()
{
	FRotator ControllerRotation = Controller->GetControlRotation();
	ControllerRotation.Yaw = SpringArmComp->GetComponentRotation().Yaw;
	Controller->SetControlRotation(ControllerRotation);
}

void ABasePlayerCharacter::MoveForward_Implementation(float Value)
{
	MoveForwardCommon(Value);
}

void ABasePlayerCharacter::MoveForwardCommon(float Value)
{
	if (IsDamaged || Animation->IsRolling)
	{
		return;
	}
	if (Value != 0.0)
	{
		auto SpeedRate = Animation->IsBlocking ? 0.3 * Speed : Speed;
		Value = SpeedRate * Value;
		ControllerRotateToCameraDirection();
		if (!Animation->IsRolling)
		{
			SetMeshRotation(Value > 0.0 ? 0 : -180);
		}
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABasePlayerCharacter::MoveRight_Implementation(float Value)
{
	MoveRightCommon(Value);
}


void ABasePlayerCharacter::MoveRightCommon(float Value)
{
	if (IsDamaged || Animation->IsRolling)
	{
		return;
	}
	if (Value != 0.0)
	{
		auto SpeedRate = Animation->IsBlocking ? 0.3 * Speed : Speed;
		Value = SpeedRate * Value;
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		ControllerRotateToCameraDirection();
		if (!Animation->IsRolling)
		{
			SetMeshRotation(Value < 0.0 ? -90 : 90);
		}
		AddMovementInput(Direction, Value);
	}
}

void ABasePlayerCharacter::Roll()
{
	if (!Animation->IsRolling && !Animation->IsFalling && !IsDamaged)
	{
		Animation->PlayRollAnimation();
	}
}

void ABasePlayerCharacter::StartAttack_Implementation()
{
	StartAttackCommon();
}

void ABasePlayerCharacter::StartAttackCommon()
{
	if (Animation->IsRolling || IsDamaged)
	{
		return;
	}
	if (Animation)
	{
		SetMeshRotation(0.0);
		Animation->IsAttacking = true;
		Animation->PlayAttackAnimation();
	}
}

void ABasePlayerCharacter::EndAttack()
{
	if (Animation)
	{
		Animation->IsAttacking = false;
	}
}

void ABasePlayerCharacter::StartBlock()
{
	if (Animation->IsRolling || IsDamaged || Animation->IsFalling)
	{
		return;
	}
	if (Animation)
	{
		Animation->IsBlocking = true;
		Animation->StopAllMontages(0.25);
	}
}

void ABasePlayerCharacter::EndBlock()
{
	if (Animation)
	{
		Animation->IsBlocking = false;
	}
}

void ABasePlayerCharacter::YawCamera(float Val)
{
	FRotator NewRotation = SpringArmComp->GetComponentRotation();
	NewRotation.Yaw += Val;
	SpringArmComp->SetWorldRotation(NewRotation);
}

void ABasePlayerCharacter::PitchCamera(float Val)
{
	FRotator NewRotation = SpringArmComp->GetComponentRotation();
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch - Val, -60.0f, -15.0f);
	SpringArmComp->SetWorldRotation(NewRotation);
}

float ABasePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                       AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDamaged)
	{
		return 0.0;
	}
	if (Health <= 0.0)
	{
		return 0.0;
	}
	if (Animation)
	{
		IsDamaged = true;
		const float MeshInitalYaw = -90.0;
		//Block attack
		{
			auto ForwardVector = GetCapsuleComponent()->GetForwardVector().GetSafeNormal().RotateAngleAxis(
				GetMesh()->GetComponentRotation().Yaw - MeshInitalYaw, FVector(0, 0, 1));
			auto FaceVector = (DamageCauser->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			auto Angle = acosf(FVector::DotProduct(ForwardVector, FaceVector));

			auto BackTransaltion = (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal();
			AddMovementInput(BackTransaltion, 20);
			if (Animation->IsBlocking && abs(Angle) < PI / 2)
			{
				IsDamaged = false;
				Animation->PlayBlockReactAnimation();
				return 0.0;
			}
		}
		//Rotate to attacker
		{
			auto LookAtRotationYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
			                                                                DamageCauser->GetActorLocation()).Yaw
				+ MeshInitalYaw;
			FRotator LookAtRotation(0.0, LookAtRotationYaw, 0.0);
			GetMesh()->SetWorldRotation(LookAtRotation);
		}

		//Cause damage.
		{
			DecreaseHealth(DamageAmount);
			SetHealthBar(Health);
		}
		//Play animation
		{
			if (Health <= 0.0)
			{
				Animation->PlayDieAnimation();
			}
			else
			{
				Animation->PlayDamagedAnimation();
			}
		}
	}
	return DamageAmount;
}

void ABasePlayerCharacter::DecreaseHealth(float Val)
{
	Health -= Val;
}

