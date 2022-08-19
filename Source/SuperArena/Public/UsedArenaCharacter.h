// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "TheBall.h"

#include "UsedArenaCharacter.generated.h"
class APowerUp;
UCLASS(config = Game)
class AUsedArenaCharacter : public ACharacter
{
	GENERATED_BODY()

		/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	AActor* CapturedBall = nullptr;

	bool MagnifiedBall = false;

	FTimerHandle MagnifyTimer;

	FTimerHandle BiggerMeTimer;

	bool MagnifiedMe = false;
public:
	AUsedArenaCharacter();

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastForcePush();



	UFUNCTION(Server, Reliable)
		void ServerForcePush();

	UFUNCTION(Server, Reliable)
		void ServerTryToMagnifyTheBall();

	UFUNCTION(Server, Reliable)
		void ServerBoostYourSpeed();

	UFUNCTION(Server, Reliable)
		void ServerTryToMagnifyMe();

	UFUNCTION(Server, Reliable)
		void ServerResetMe();
	UFUNCTION(Server, Reliable)
		void ServerResetTheBall();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
		float TurnRateGamepad;

	//ForceDistance from the emitter
	UPROPERTY(EditAnywhere)
		float ForceDistanceOffset = 100;
	//How hard you hit the ball
	UPROPERTY(EditAnywhere)
		float ForcePower = 7500;

	UPROPERTY(EditAnywhere)
		float BoostPowerCoe =100 ;

	UPROPERTY(EditAnywhere)
		float BoostSpeedZValue = 100;


	UPROPERTY(EditAnywhere)
		float BigScale = 3;
	//VFX part
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* VFX_ForcePush;
	//Time used to reset the ball after scaling
	UPROPERTY(EditAnywhere)
		float TimeToResetTheBallSize = 2;

	UPROPERTY(EditAnywhere)
		float TimeToResetMe = 3;
	//how bigger or smaller you scale the ball
	UPROPERTY(EditAnywhere)
		float RelativeScaling3DCoe = 2;

	UPROPERTY(EditAnywhere)
		float RelativeScalingMe3DCoe = 2;
	void ForcePush();
	void SprintStart();
	void SprintEnd();
	
	//User Item
	void UseItem();
	UFUNCTION(Server, Reliable)
	void ServerUseItem();
	APowerUp* CurrentPowerUp=nullptr;


	void TryToManifyMe();
	void ResetMe();

	void TryToMagnifyTheBall();
	void ResetTheBall();
	
	void BoostYourSpeed();

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

