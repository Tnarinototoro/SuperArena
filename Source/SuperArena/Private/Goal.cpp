// Fill out your copyright notice in the Description page of Project Settings.


#include "Goal.h"

// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal Mesh"));
	GoalMesh->SetupAttachment(RootComponent);

	GoalCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	GoalCollider->SetupAttachment(GoalMesh);
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
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Goal overlapped with something"));
		auto GotBall = Cast<ATheBall>(OtherActor);
		if (GotBall)
		{
			GotBall->Destroy();
			GameState->TeamOneGoalScored(1);
			auto SpawnedActor=GetWorld()->SpawnActor(BP_BallClass, &BallSpawnLocation);
			SpawnedActor->GetRootComponent()->ComponentTags.Add(FName("Ball"));
			UE_LOG(LogTemp, Warning, TEXT("%d"), GameState->TeamOneScore);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Not A Ball"));
		}
	}
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

