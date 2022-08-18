// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameState.h"

AArenaGameState::AArenaGameState()
{
	this->TeamOneScore = 0;
	this->TeamTwoScore = 0;
}

void AArenaGameState::TeamOneGoalScored(int amount)
{
	if(HasAuthority())
	this->TeamOneScore += amount;
}

void AArenaGameState::TeamTwoGoalScored(int amount)
{
	if(HasAuthority())
	this->TeamTwoScore += amount;
}

void AArenaGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AArenaGameState, TeamOneScore);
	DOREPLIFETIME(AArenaGameState, TeamTwoScore);

}

