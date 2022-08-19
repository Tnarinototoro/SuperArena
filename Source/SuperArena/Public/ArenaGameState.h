// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "ArenaPlayerState.h"
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

	UPROPERTY(Replicated)
		int TeamOneScore;
	UPROPERTY(Replicated)
		int TeamTwoScore;
	UPROPERTY(EditAnywhere)
	TArray<AArenaPlayerState*> TeamOne;
	UPROPERTY(EditAnywhere)
	TArray<AArenaPlayerState*> TeamTwo;
	UPROPERTY(EditAnywhere)
	FLinearColor TeamOneColour;
	UPROPERTY(EditAnywhere)
	FLinearColor TeamTwoColour;

	virtual void AddPlayerState
	(APlayerState* PlayerState) override
		;
	void TeamOneGoalScored(int amount);
	void TeamTwoGoalScored(int amount);
void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
