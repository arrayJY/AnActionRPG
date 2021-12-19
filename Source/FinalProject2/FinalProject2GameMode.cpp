// Copyright Epic Games, Inc. All Rights Reserved.

#include "FinalProject2GameMode.h"
#include "FinalProject2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "BasePlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AFinalProject2GameMode::AFinalProject2GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/Blueprints/PlayerCharacter_"));
	static ConstructorHelpers::FClassFinder<APawn> MeleeEnemyBP(
		TEXT("/Game/Enemy/Melee/Blueprints/MeleeEnemyCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> ArcherEnemyBP(
		TEXT("/Game/Enemy/Archer/Blueprints/ArcherEnemyCharacter"));
	MeleeEnemyBPClass = MeleeEnemyBP.Class;
	ArcherEnemyBPClass = ArcherEnemyBP.Class;
	Spawner = CreateDefaultSubobject<AEnemySpawner>("Spawner");
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AFinalProject2GameMode::BeginPlay()
{
	Super::BeginPlay();
	RandomStream.GenerateNewSeed();
	for (int i = 0; i < 10; i++)
	{
		RandomSpawnAnEnemy();
	}

	auto Player = Cast<ABasePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerController = Player->GetController();
}


ABasePlayerCharacter* AFinalProject2GameMode::RandomSpawnAnEnemy(float ATK, float DEF)
{
	UClass* EnmeyBPClass = RandomStream.RandRange(0, 1) ? MeleeEnemyBPClass : ArcherEnemyBPClass;
	ABasePlayerCharacter* Enemy = nullptr;
	if (Spawner && EnmeyBPClass)
	{
		auto X = RandomStream.FRandRange(-1000, 1000);
		auto Y = RandomStream.FRandRange(-1000, 1000);
		auto Z = RandomStream.FRandRange(300, 400);
		Enemy = Spawner->SpawnEnemy(EnmeyBPClass, FVector(X, Y, Z), ATK, DEF);
		if (Enemy)
		{
			Enemy->OnDestroyed.AddDynamic(this, &AFinalProject2GameMode::OnEnemyDied);
		}
	}
	return Enemy;
}

void AFinalProject2GameMode::OnEnemyDied(AActor* Actor)
{
	auto Enemy = Cast<ABasePlayerCharacter>(Actor);
	if (Enemy)
	{
		auto Player = Cast<ABasePlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		auto ATK = Enemy->ATKValue, DEF = Enemy->DEFRate;
		Player->IncreaseScore(ATK, DEF);
		ABasePlayerCharacter* NewEnemy = nullptr;
		do
		{
			NewEnemy = RandomSpawnAnEnemy(ATK * 1.1, DEF * 0.95);
		}
		while (!NewEnemy);
	}
}

void AFinalProject2GameMode::Restart()
{
	RestartPlayer(PlayerController);
}
