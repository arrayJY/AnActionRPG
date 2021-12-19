// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowActor.h"

#include "BasePlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AArrowActor::AArrowActor()
{
	HitTime = 0;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent)
	{
		// 用球体进行简单的碰撞展示。
		CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
		// 设置球体的碰撞半径。
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Arrow"));
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AArrowActor::OnOverlapBegin);
		CollisionComponent->InitBoxExtent(FVector(25, 1, 1));
		// 将根组件设置为碰撞组件。
		RootComponent = CollisionComponent;
	}
	if (!ProjectileMovementComponent)
	{
		// 使用此组件驱动发射物的移动。
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
			TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);

		// ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(TEXT("StaticMesh'/Game/Weapon/Arrow.Arrow'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
		ProjectileMeshComponent->SetupAttachment(RootComponent);
		ProjectileMeshComponent->AddWorldRotation(FRotator(90, 0, 0));
		ProjectileMeshComponent->AddWorldOffset(FVector(25, 0, 0));
	}
	InitialLifeSpan = 3.0f;
}

void AArrowActor::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}


void AArrowActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	HitTime++;
	if (OtherActor != this && OtherActor->GetInstigator() != GetInstigator() )
	{

		if(HitTime == 1)
		{
			auto Character = Cast<ABasePlayerCharacter>(GetOwner());
			if(Character)
			{
				UGameplayStatics::ApplyDamage(OtherActor, Character->ATKValue, nullptr, GetOwner(), nullptr);
			}
		}
		CollisionComponent->SetSimulatePhysics(false);
		ProjectileMovementComponent->StopMovementImmediately();
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called when the game starts or when spawned
void AArrowActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AArrowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
