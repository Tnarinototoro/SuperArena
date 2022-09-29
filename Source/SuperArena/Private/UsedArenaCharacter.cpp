// Copyright Epic Games, Inc. All Rights Reserved.

#include "UsedArenaCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PowerUp.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

//////////////////////////////////////////////////////////////////////////
// AUsedArenaCharacter
AUsedArenaCharacter::AUsedArenaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}
//Sprint
void AUsedArenaCharacter::SprintStart()
{
	//UE_LOG(LogTemp, Warning, TEXT("SprintStart"));
	auto moveCompo = GetCharacterMovement();
	if (moveCompo != nullptr)
	{
		moveCompo->MaxWalkSpeed = 780.f;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SprintStart Movement Compo NULL"));
	}
}
void AUsedArenaCharacter::SprintEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("SprintEnd"));
	auto moveCompo = GetCharacterMovement();
	if (moveCompo != nullptr)
	{
		moveCompo->MaxWalkSpeed = 500.f;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SprintEnd Movement Compo NULL"));
	}
}

//Boost yourself
void AUsedArenaCharacter::BoostYourSpeed()
{
	ServerBoostYourSpeed();
}
void AUsedArenaCharacter::ServerBoostYourSpeed_Implementation()
{
	if (CanWeBoost)
	{
		CanWeBoost = false;
		auto MovementCompo = GetCharacterMovement();
		FVector Temp = this->GetMesh()->GetRightVector() * (BoostPowerCoe);
		Temp.Z = BoostSpeedZValue;
		MovementCompo->SetMovementMode(EMovementMode::MOVE_Falling);
		MovementCompo->Velocity = Temp;
		GetWorld()->GetTimerManager().SetTimer(BoostResetTimer, this, &AUsedArenaCharacter::ResetBoost, CooldownTime);
	}
	
}
void AUsedArenaCharacter::ResetBoost()
{
	ServerResetBoost();
}
void AUsedArenaCharacter::ServerResetBoost_Implementation()
{
	MulticastTryResetBoost();
}
void AUsedArenaCharacter::MulticastTryResetBoost_Implementation()
{
	CanWeBoost = true;
}
//Force Push 
//For generating particle effects for every hit
void AUsedArenaCharacter::ForcePush()
{
	ServerForcePush();
}
void AUsedArenaCharacter::MulticastForcePush_Implementation()
{
	TArray<FHitResult> BallHitResults;
	const FVector StartPoint = this->GetActorLocation();
	const FVector EndPoint = StartPoint + this->GetActorForwardVector() * ForceDistanceOffset;
	UNiagaraFunctionLibrary::
		SpawnSystemAtLocation(
			GetWorld(),
			VFX_ForcePush,
			EndPoint,
			this->GetActorRotation());
}
void AUsedArenaCharacter::ServerTryToMagnifyTheBall_Implementation()
{
	bool GotBall = (CapturedBall != nullptr);
	if (MagnifiedBall)
	{

	}
	else
	{

		TArray<FHitResult> BallHitResults;
		const FVector StartPoint = this->GetActorLocation();
		const FVector EndPoint = StartPoint + this->GetActorForwardVector() * ForceDistanceOffset;
		const FCollisionShape CubicShape = FCollisionShape::MakeBox(FVector(ForceDistanceOffset));
		const bool Sweep = GetWorld()->
			SweepMultiByChannel
			(BallHitResults, StartPoint, EndPoint,
				this->GetActorQuat(),
				ECC_WorldDynamic, CubicShape);
		if (Sweep)
		{

			for (const auto& HitResult : BallHitResults)
			{
				auto ActorGot = HitResult.GetActor();
				if (ActorGot != nullptr && ActorGot != this)
				{
					UStaticMeshComponent* HitCompo =
						Cast<UStaticMeshComponent>
						(ActorGot->GetRootComponent());

					if (HitCompo == nullptr)
					{
						return;
					}
					if (Cast<ATheBall>(ActorGot))
					{

						MagnifiedBall = true;
						CapturedBall = ActorGot;
						CapturedBall->SetActorRelativeScale3D(FVector(RelativeScaling3DCoe));
						
						auto rootMesh=Cast<UStaticMeshComponent>(CapturedBall->GetRootComponent());
						if (rootMesh)
						{
							rootMesh->SetMassScale(NAME_None,100);
						}
						GetWorld()->GetTimerManager().SetTimer(MagnifyTimer, this, &AUsedArenaCharacter::ResetTheBall, TimeToResetTheBallSize, false, -1);
						break;
					}
				}
			}

		}
	}
}
void AUsedArenaCharacter::ServerResetTheBall_Implementation()
{
	MagnifiedBall = false;
	auto rootMesh = Cast<UStaticMeshComponent>(CapturedBall->GetRootComponent());
	if (rootMesh)
	{
		rootMesh->SetMassScale(NAME_None, 1);
	}

	CapturedBall->SetActorScale3D(FVector(1));
	CapturedBall = nullptr;
}
void AUsedArenaCharacter::ServerForcePush_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ForcePush"));
	MulticastForcePush();
	TArray<FHitResult> BallHitResults;
	const FVector StartPoint = this->GetActorLocation();
	const FVector EndPoint = StartPoint + this->GetActorForwardVector() * ForceDistanceOffset;
	const FCollisionShape CubicShape = FCollisionShape::MakeBox(FVector(ForceDistanceOffset));
	const bool Sweep = GetWorld()->
		SweepMultiByChannel
		(BallHitResults, StartPoint, EndPoint,
			this->GetActorQuat(),
			ECC_WorldDynamic, CubicShape);
	if (Sweep && VFX_ForcePush != nullptr)
	{

		for (const auto& HitResult : BallHitResults)
		{
			auto ActorGot = HitResult.GetActor();
			if (ActorGot != nullptr && ActorGot != this)
			{
				UStaticMeshComponent* HitCompo =
					Cast<UStaticMeshComponent>
					(ActorGot->GetRootComponent());

				if (HitCompo == nullptr)
				{
					return;
				}
				if (Cast<ATheBall>(ActorGot))
				{
					HitCompo->AddRadialImpulse(StartPoint,
						ForceDistanceOffset * 2,
						ForcePower, ERadialImpulseFalloff::RIF_Linear,
						true
					);


					break;
				}
			}
		}

	}
}
//Fire beam
void AUsedArenaCharacter::FireBeam()
{
	ServerFireBeam();
}
void AUsedArenaCharacter::MulticastFireBeam_Implementation()
{
	if (VFX_fireBeam != nullptr)
	{
		FVector InnerBeamSize = FVector(0.2, 0.2, 5);
		FVector OuterBeamSize = FVector(0.5, 0.5, 5);
		FVector StartTrace = this->GetActorLocation() + (this->GetActorForwardVector() * 100);
		FVector EndTrace = this->GetActorLocation() + (this->GetActorForwardVector() * 5000);
		FHitResult TraceResults;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		GetWorld()->LineTraceSingleByChannel(TraceResults, StartTrace, EndTrace, ECC_Pawn, TraceParams);
		if (TraceResults.GetActor()) 
		{
			float Distance = TraceResults.Distance / 100;
			InnerBeamSize.Z = Distance;
			OuterBeamSize.Z = Distance;
		}

		UNiagaraComponent* LaserComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
				VFX_fireBeam, StartTrace,
				this->GetActorRotation());
		LaserComponent->SetNiagaraVariableVec3(FString("InnerBeamScale"), InnerBeamSize);
		LaserComponent->SetNiagaraVariableVec3(FString("OuterBeamScale"), OuterBeamSize);

	}
}
void AUsedArenaCharacter::ServerFireBeam_Implementation()
{
	MulticastFireBeam();
}

//Scanning highlight functions
void AUsedArenaCharacter::Scan()
{
	ServerScan();
	
}
void AUsedArenaCharacter::ServerScan_Implementation()
{

	MulticastServerScan();


}
void AUsedArenaCharacter::MulticastServerScan_Implementation()
{
	TArray<FHitResult> BallHitResults;
	const FVector StartPoint = this->GetActorLocation();
	const FVector EndPoint = StartPoint + this->GetActorForwardVector() * ScanRadius;
	const FCollisionShape CubicShape = FCollisionShape::MakeBox(FVector(ScanRadius));
	const bool Sweep = GetWorld()->
		SweepMultiByChannel
		(BallHitResults, StartPoint, EndPoint,
			this->GetActorQuat(),
			ECC_WorldDynamic, CubicShape);
	if (Sweep)
	{

		for (const auto& HitResult : BallHitResults)
		{
			auto ActorGot = HitResult.GetActor();
			if (ActorGot != nullptr && ActorGot != this)
			{
				UStaticMeshComponent* HitCompo =
					Cast<UStaticMeshComponent>
					(ActorGot->GetRootComponent());

				if (HitCompo && Cast<APowerUp>(ActorGot))
				{
					StoredObjectsHighlighted.Add(HitCompo);
					HitCompo->SetRenderCustomDepth(true);
					HitCompo->SetCustomDepthStencilValue(0);

				}
			}
		}
		GetWorld()->
			GetTimerManager().SetTimer(
				HighlightTimer,
				this,
				&AUsedArenaCharacter::CancelHightLight,
				ResetScanningTimer,
				false,
				-1);

	}
}
void AUsedArenaCharacter::CancelHightLight()
{
	ServerCancelHightLight();
	
}
void AUsedArenaCharacter::MulticastServerCancelHightLight_Implementation()
{
	if (StoredObjectsHighlighted.Num() == 0)
		return;
	for (auto p : StoredObjectsHighlighted)
	{
		if (p)
		{
			p->SetRenderCustomDepth(false);
		}
	}
	StoredObjectsHighlighted.Empty();
}
void AUsedArenaCharacter::ServerCancelHightLight_Implementation()
{
	MulticastServerCancelHightLight();

	
}

//Use item function
void AUsedArenaCharacter::UseItem()
{
	ServerUseItem();
}
void AUsedArenaCharacter::ServerUseItem_Implementation()
{
	if(CurrentPowerUp)
	{
		UE_LOG(LogTemp, Warning, TEXT("A player used an item!"));
		CurrentPowerUp->Use(this);
		CurrentPowerUp = nullptr;

	}

}

//make yourself bigger
void AUsedArenaCharacter::TryToManifyMe()
{
	ServerTryToMagnifyMe();
}
void AUsedArenaCharacter::ServerTryToMagnifyMe_Implementation()
{

	MulticastTryToMagnifyMe();

}
void AUsedArenaCharacter::ServerResetMe_Implementation()
{
	MulticastTryToRestMe();

}
void AUsedArenaCharacter::ResetMe()
{
	ServerResetMe();
}
void AUsedArenaCharacter::MulticastTryToRestMe_Implementation()
{
	MagnifiedMe = false;
	this->SetActorRelativeScale3D(FVector(1));
}
void AUsedArenaCharacter::MulticastTryToMagnifyMe_Implementation()
{

	if (MagnifiedMe)
	{

	}
	else
	{

		MagnifiedMe = true;
		this->SetActorRelativeScale3D(FVector(RelativeScalingMe3DCoe));
		GetWorld()->GetTimerManager().SetTimer(BiggerMeTimer, this, &AUsedArenaCharacter::ResetMe, TimeToResetMe, false, -1);

	}
}
//make the ball bigger
void AUsedArenaCharacter::TryToMagnifyTheBall()
{

	ServerTryToMagnifyTheBall();
}
void AUsedArenaCharacter::ResetTheBall()
{
	ServerResetTheBall();

}

//Input
void AUsedArenaCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("ForcePush", IE_Pressed, this, &AUsedArenaCharacter::ForcePush);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AUsedArenaCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AUsedArenaCharacter::SprintEnd);
	PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AUsedArenaCharacter::UseItem);
	PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &AUsedArenaCharacter::BoostYourSpeed);
	PlayerInputComponent->BindAction("Bigger", IE_Pressed, this, &AUsedArenaCharacter::TryToManifyMe);
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AUsedArenaCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AUsedArenaCharacter::MoveRight);

	PlayerInputComponent->BindAction("FireBeam", IE_Pressed, this, &AUsedArenaCharacter::FireBeam);
	PlayerInputComponent->BindAction("MagnifyTheBall", IE_Pressed, this, &AUsedArenaCharacter::TryToMagnifyTheBall);
	PlayerInputComponent->BindAction("Scan", IE_Pressed, this, &AUsedArenaCharacter::Scan);
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AUsedArenaCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AUsedArenaCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AUsedArenaCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AUsedArenaCharacter::TouchStopped);
}
void AUsedArenaCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}
void AUsedArenaCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}
void AUsedArenaCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}
void AUsedArenaCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}
void AUsedArenaCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void AUsedArenaCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}