// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ArenaGameState.h"
#include "CommonScoreWidget.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT)
class SUPERARENA_API UCommonScoreWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
		UTextBlock* TeamOneScoreRef;

	UPROPERTY(EditAnywhere)
		UTextBlock* TeamTwoScoreRef;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
		void RealInit();
	UFUNCTION(BlueprintCallable)
		void RealTick();

	virtual void NativeTick(const FGeometry& WidgetGeometry, float IndeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void SetTextRef(UTextBlock* inputOne, UTextBlock* inputTwo)
	{
		TeamOneScoreRef = inputOne;
		TeamTwoScoreRef = inputTwo;
	}
protected:
	AArenaGameState* GameStateOfGame;
};
