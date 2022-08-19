// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiBallPowerUp.h"

void AMultiBallPowerUp::Use(AUsedArenaCharacter* OwningPlayer)
{
	Super::Use(OwningPlayer);

	if(TheBallClass)
	{

		GetWorld()->SpawnActor(TheBallClass, &BallSpawnLocation);

	}

}
