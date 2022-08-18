// Fill out your copyright notice in the Description page of Project Settings.


#include "TheBall.h"

// Sets default values
ATheBall::ATheBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	if(BallMesh!=nullptr)
	SetRootComponent(BallMesh);
	BallMesh->SetSimulatePhysics(true);
	bReplicates = true;
	BallMesh->SetIsReplicated(true);
	NetPriority = 3.0f;
}

// Called when the game starts or when spawned
void ATheBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATheBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

