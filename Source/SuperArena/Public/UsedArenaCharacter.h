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

	FTimerHandle HighlightTimer;

	FTimerHandle BoostResetTimer;

	bool MagnifiedMe = false;
public:

	
	void SprintStart();
	void SprintEnd();


	//scan radius
	
	void Scan();
	void CancelHightLight();

	UPROPERTY(EditAnywhere)
	float ResetScanningTimer = 2.0f;
	UPROPERTY(EditAnywhere)
	float ScanRadius = 200.0f;
	UFUNCTION(Server, Reliable)
	void ServerScan();
	UFUNCTION(Server, Reliable)
	void ServerCancelHightLight();
	TArray<UStaticMeshComponent*> StoredObjectsHighlighted;

	APowerUp* CurrentPowerUp = nullptr;


	
	

	
	

	
	AUsedArenaCharacter();

	//Force push functions
	void ForcePush();
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastForcePush();
	UFUNCTION(Server, Reliable)
		void ServerForcePush();

	//fire beam
	void FireBeam();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastFireBeam();
	UFUNCTION(Server, Reliable)
		void ServerFireBeam();


	//MagnifyTheBall Functions there is no net
	//version functions becaue the ball has been replicated
	void TryToMagnifyTheBall();
	UFUNCTION(Server, Reliable)
		void ServerTryToMagnifyTheBall();

	void ResetTheBall();
	UFUNCTION(Server, Reliable)
		void ServerResetTheBall();

	//Boost yourself
	UFUNCTION(Server, Reliable)
		void ServerBoostYourSpeed();
	void BoostYourSpeed();

	//Magnify me 
	void TryToManifyMe();
	UFUNCTION(Server, Reliable)
		void ServerTryToMagnifyMe();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastTryToMagnifyMe();

	void ResetMe();
	UFUNCTION(Server, Reliable)
		void ServerResetMe();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastTryToRestMe();

	void ResetBoost();
	UFUNCTION(Server, Reliable)
		void ServerResetBoost();
	UFUNCTION(NetMulticast, Unreliable)
		void MulticastTryResetBoost();

	//User Item
	void UseItem();
	UFUNCTION(Server, Reliable)
		void ServerUseItem();
	

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
		float CooldownTime = 2.0f;
	
		bool CanWeBoost = true;

	UPROPERTY(EditAnywhere)
		float BigScale = 3;
	//VFX part
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* VFX_ForcePush;
	//Time used to reset the ball after scaling

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* VFX_fireBeam;

	UPROPERTY(EditAnywhere)
		float TimeToResetTheBallSize = 2;

	UPROPERTY(EditAnywhere)
		float TimeToResetMe = 3;
	//how bigger or smaller you scale the ball
	UPROPERTY(EditAnywhere)
		float RelativeScaling3DCoe = 2;

	UPROPERTY(EditAnywhere)
		float RelativeScalingMe3DCoe = 2;
	

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


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


	
};

