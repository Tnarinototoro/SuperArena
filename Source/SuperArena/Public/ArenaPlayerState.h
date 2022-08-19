// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "ArenaPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SUPERARENA_API AArenaPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void ClientInitialize(AController* C) override;
};
