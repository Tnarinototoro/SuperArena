// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"
#include "UsedArenaCharacter.h"
// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal Mesh"));
	GoalMesh->SetupAttachment(RootComponent);

	GoalCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	GoalCollider->SetupAttachment(GoalMesh);
	bReplicates = true;
}

void AGoal::ServerSpawnGoalParticles_Implementation(FVector SpawnLocation, bool TeamOne)
{
	MulticastSpawnGoalParticles(SpawnLocation, TeamOne);
}

void AGoal::MulticastSpawnGoalParticles_Implementation(FVector SpawnLocation, bool TeamOne)
{
	if (VFX_GoalExplosion)
	{
		UNiagaraComponent* GoalExplosion = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFX_GoalExplosion, SpawnLocation, GetActorForwardVector().Rotation());
		if (TeamOne)
		{
			GoalExplosion->SetNiagaraVariableLinearColor(FString("User.Colour"), GameState->TeamOneColour);
		}
		else
		{
			GoalExplosion->SetNiagaraVariableLinearColor(FString("User.Colour"), GameState->TeamTwoColour);
		}
	}

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();
	GoalCollider->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnBeginOverlap);
	GameState = Cast<AArenaGameState>(GetWorld()->GetGameState());
	
}

void AGoal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		if (OtherActor && OtherActor != this)
		{
			UE_LOG(LogTemp, Warning, TEXT("Goal overlapped with something"));
			auto GotBall = Cast<ATheBall>(OtherActor);
			if (GotBall)
			{
				if (GotBall->LastPlayer)
				{
					if(GameState) 
					{
						if
							(GameState->TeamOne.Contains(GotBall->LastPlayer->GetPlayerState()))
						{
							GameState->TeamOneGoalScored(1);
							ServerSpawnGoalParticles(GotBall->GetActorLocation(), true);

						}
						else
						{
							GameState->TeamTwoGoalScored(1);
							ServerSpawnGoalParticles(GotBall->GetActorLocation(), false);
						}
					}
				}
				GotBall->Destroy();
				GetWorld()->SpawnActor
				(BP_BallClass, &BallSpawnLocation);

			}
			
			
			

		}
	}
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

