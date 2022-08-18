// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameState.h"

AArenaGameState::AArenaGameState()
{
	this->TeamOneScore = 0;
	this->TeamTwoScore = 0;
}

void AArenaGameState::TeamOneGoalScored(int amount)
{
	this->TeamOneScore += amount;
}

void AArenaGameState::TeamTwoGoalScored(int amount)
{
	this->TeamTwoScore += amount;
}

