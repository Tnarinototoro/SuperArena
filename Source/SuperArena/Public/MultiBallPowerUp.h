// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp.h"
#include "TheBall.h"
#include "MultiBallPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class SUPERARENA_API AMultiBallPowerUp : public APowerUp
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FVector BallSpawnLocation;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ATheBall> TheBallClass;
	virtual void Use
	(AUsedArenaCharacter* OwningPlayer) override;


};
