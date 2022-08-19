// Fill out your copyright notice in the Description page of Project Settings.


#include "ArenaGameState.h"

AArenaGameState::AArenaGameState()
{
	this->TeamOneScore = 0;
	this->TeamTwoScore = 0;
	TeamOneColour = FLinearColor::Blue;
	TeamTwoColour = FLinearColor::Red;
}

void AArenaGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
	if(Cast<AArenaPlayerState>(PlayerState))
	{
		AArenaPlayerState* NewArenaPlayerState = Cast
			<AArenaPlayerState>(PlayerState);
		if(TeamOne.Num() < TeamTwo.Num())
		{
			TeamOne.Add(NewArenaPlayerState);
		}
		else if(TeamTwo.Num() < TeamOne.Num())
		{
			TeamTwo.Add(NewArenaPlayerState);
		}
		else
		{
			if
				(rand() % 2 == 0)
			{
				TeamOne.Add(NewArenaPlayerState);
			}
			else
			{
				TeamTwo.Add(NewArenaPlayerState);
			}
		}
	}

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

