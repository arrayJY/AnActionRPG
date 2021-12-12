// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerCharacter.h"

// Sets default values
ABasePlayerCharacter::ABasePlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	check(FPSCameraComponent != nullptr);

	// 将摄像机组件附加到我们的胶囊体组件。
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent, UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 100.0f + BaseEyeHeight));
	FPSCameraComponent->bUsePawnControlRotation = true;
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
	PlayerInputComponent->BindAxis("Turn", this, &ABasePlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasePlayerCharacter::AddControllerPitchInput);

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
	if (Animation)
	{
		Value = Animation->IsBlocking ? 0.3 * Value : Value;
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABasePlayerCharacter::MoveRight(float Value)
{
	if (Animation)
	{
		Value = Animation->IsBlocking ? 0.3 * Value : Value;
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
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

float ABasePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Damaged"));
	if(Animation)
	{
		Animation->PlayDamagedAnimation();
	}
	return DamageAmount;
}

