// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ArenaGameState.generated.h"

/**
 * 
 */
UCLASS()
class SUPERARENA_API AArenaGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AArenaGameState();

	UPROPERTY()
		int TeamOneScore;
	UPROPERTY()
		int TeamTwoScore;

	void TeamOneGoalScored(int amount);
	void TeamTwoGoalScored(int amount);
};
