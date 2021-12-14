// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "WithWeaponSceneComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALPROJECT2_API UWithWeaponSceneComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UWithWeaponSceneComponent();
private:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Weapon, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* HitBox;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);
	/*
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	                  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	                  */
};
