// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaPlayerState.h"

void AArenaPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	if (HasAuthority())
	{
		GetWorld()->GetGameState()->AddPlayerState(this);
	}
}
