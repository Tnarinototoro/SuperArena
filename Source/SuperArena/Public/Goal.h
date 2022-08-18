// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "TheBall.h"
#include "ArenaGameState.h"
#include "Goal.generated.h"

UCLASS()
class SUPERARENA_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* GoalMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* GoalCollider;

	UPROPERTY(EditAnywhere, Category = "Ball_Spawning")
		FVector BallSpawnLocation;
	
	UPROPERTY(EditAnywhere, Category = "Ball_Spawning")
		TSubclassOf<ATheBall> BP_BallClass;
protected:
	AArenaGameState* GameState;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, 
			UPrimitiveComponent* OtherComponent, 
			int32 OtherBodyIndex, 
			bool bFromSweep, 
			const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
