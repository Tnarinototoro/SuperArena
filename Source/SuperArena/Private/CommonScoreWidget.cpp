// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonScoreWidget.h"

void UCommonScoreWidget::NativeConstruct()
{
	
}

void UCommonScoreWidget::RealInit()
{
	
	GameStateOfGame =
		Cast<AArenaGameState>(GetWorld()->GetGameState());
}

void UCommonScoreWidget::RealTick()
{
	if (GameStateOfGame)
	{
		
		TeamOneScoreRef->
			SetText
			(FText::AsNumber
			(GameStateOfGame->TeamOneScore));
		TeamTwoScoreRef->
			SetText(FText::AsNumber
			(GameStateOfGame->TeamTwoScore));
	}
}


void UCommonScoreWidget::NativeTick(const FGeometry& WidgetGeometry, float IndeltaTime)
{
	
	
}
