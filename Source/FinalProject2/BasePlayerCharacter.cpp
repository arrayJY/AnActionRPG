// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(60.0f, 0.0f, 0.0f));
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasePlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABasePlayerCharacter::StopJump);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABasePlayerCharacter::StartAttack);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABasePlayerCharacter::EndAttack);
	PlayerInputComponent->BindAction("Block", IE_Pressed, this, &ABasePlayerCharacter::StartBlock);
	PlayerInputComponent->BindAction("Block", IE_Released, this, &ABasePlayerCharacter::EndBlock);
}


void ABasePlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0)
	{
		const float MeshInitDegree = -90.0;
		Value = Animation->IsBlocking ? 0.3 * Value : Value;
		FRotator ControllerRotation = Controller->GetControlRotation();
		FRotator MeshRotation = GetMesh()->GetComponentRotation();
		ControllerRotation.Yaw = SpringArmComp->GetComponentRotation().Yaw;
		MeshRotation.Yaw = ControllerRotation.Yaw + MeshInitDegree;
		MeshRotation.Yaw -= Value < 0.0 ? -180.0 : 0.0;
		Controller->SetControlRotation(ControllerRotation);
		GetMesh()->SetWorldRotation(MeshRotation);
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABasePlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0)
	{
		Value = Animation->IsBlocking ? 0.3 * Value : Value;
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		FRotator MeshRotation = GetMesh()->GetComponentRotation();
		MeshRotation.Yaw = SpringArmComp->GetComponentRotation().Yaw;
		MeshRotation.Yaw -= Value < 0.0 ? -180.0 : 0.0;
		GetMesh()->SetWorldRotation(MeshRotation);
		AddMovementInput(Direction, Value);
	}
}

void ABasePlayerCharacter::StartJump()
{
	bPressedJump = true;
}

void ABasePlayerCharacter::StopJump()
{
	bPressedJump = false;
}

void ABasePlayerCharacter::StartAttack()
{
	if (Animation)
	{
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
	if (Animation)
	{
		Animation->IsBlocking = true;
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
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + Val, -60.0f, -15.0f);
	SpringArmComp->SetWorldRotation(NewRotation);
}


float ABasePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                       AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Damaged"));
	if (Animation)
	{
		Animation->PlayDamagedAnimation();
	}
	return DamageAmount;
}
