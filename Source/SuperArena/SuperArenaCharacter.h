// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "SuperArenaCharacter.generated.h"

UCLASS(config=Game)
class ASuperArenaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	AActor* CapturedBall=nullptr;

	bool MagnifiedBall = false;

	FTimerHandle MagnifyTimer;
public:
	ASuperArenaCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	//ForceDistance from the emitter
	UPROPERTY(EditAnywhere)
	float ForceDistanceOffset = 100;
	//How hard you hit the ball
	UPROPERTY(EditAnywhere)
	float ForcePower = 7500;
	//VFX part
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* VFX_ForcePush;
	//Time used to reset the ball after scaling
	UPROPERTY(EditAnywhere)
	float TimeToResetTheBallSize = 2;
	//how bigger or smaller you scale the ball
	UPROPERTY(EditAnywhere)
	float RelativeScaling3DCoe = 2;
	void ForcePush();
	void SprintStart();
	void SprintEnd();

	void TryToMagnifyTheBall();
	void ResetTheBall();

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

