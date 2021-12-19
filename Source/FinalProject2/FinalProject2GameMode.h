// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "GameFramework/GameModeBase.h"
#include "FinalProject2GameMode.generated.h"

UCLASS(minimalapi)
class AFinalProject2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFinalProject2GameMode();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UClass* MeleeEnemyBPClass;

	UPROPERTY(EditAnywhere)
	UClass* ArcherEnemyBPClass;

	UPROPERTY(EditAnywhere)
	AEnemySpawner* Spawner;

	UFUNCTION()
	ABasePlayerCharacter* RandomSpawnAnEnemy(float ATK = 10.0, float DEF = 1.0);

	FRandomStream RandomStream;


	UFUNCTION()
	void OnEnemyDied(AActor* Actor);
	UFUNCTION(BlueprintCallable)
	void Restart();

	AController* PlayerController;
};
