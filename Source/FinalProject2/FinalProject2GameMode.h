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
	UClass* EnemyBPClass;
	
	UPROPERTY(EditAnywhere)
	AEnemySpawner* Spawner;
	
};



