// Fill out your copyright notice in the Description page of Project Settings.


#include "WithWeaponSceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"

// Sets default values for this component's properties
UWithWeaponSceneComponent::UWithWeaponSceneComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	if(!Weapon)
	{
		Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	}
	if(!HitBox)
	{
		HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	}
	Weapon->SetupAttachment(this);
	HitBox->SetupAttachment(this);
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &UWithWeaponSceneComponent::OnOverlapBegin);
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


// Called when the game starts
void UWithWeaponSceneComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UWithWeaponSceneComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWithWeaponSceneComponent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* Target,
                                               class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                               bool bFromSweep,
                                               const FHitResult& SweepResult)
{
	auto Attacker = OverlappedComp->GetOwner();
	if (Attacker != Target)
	{
		UGameplayStatics::ApplyDamage(Target, 10.0, nullptr, Attacker, nullptr);
	}
}
