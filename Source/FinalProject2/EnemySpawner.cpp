// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "BasePlayerCharacter.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ABasePlayerCharacter* AEnemySpawner::SpawnEnemy(UClass* PawnClass, FVector const& Location, float ATKValue,
                                                float DEFRate)
{
	if (PawnClass)
	{
		auto Pawn = GetWorld()->SpawnActor<ABasePlayerCharacter>(PawnClass, Location, FRotator(0.f));
		if (Pawn)
		{
			Pawn->SpawnDefaultController();
			Pawn->ATKValue = ATKValue;
			Pawn->DEFRate = DEFRate;
		}
		return Pawn;
	}
	return nullptr;
}
