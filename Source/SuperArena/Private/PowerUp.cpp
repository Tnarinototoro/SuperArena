// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"

// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PowerUpMesh = CreateDefaultSubobject
		<UStaticMeshComponent>(TEXT
		("Power Up Mesh"));
	PowerUpMesh->SetupAttachment
	(RootComponent);
	PowerUpMesh->OnComponentBeginOverlap
		.AddDynamic
		(this, &APowerUp::OnBeginOverlap);
	PowerUpMesh->SetCollisionProfileName
	("OverlapAll");
}

void APowerUp::PickUp(AUsedArenaCharacter* OwningPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Item Picked Up!"));
	OwningPlayer->CurrentPowerUp = this;
}

void APowerUp::Use(AUsedArenaCharacter* OwningPlayer)
{
	UE_LOG(LogTemp, Warning, TEXT("Item Used!"));
	Destroy();

}

void APowerUp::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if(Cast<AUsedArenaCharacter>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Item Overlapped with Character!"));
			PickUp(Cast<AUsedArenaCharacter>(OtherActor));
			PowerUpMesh->DestroyComponent();
		}
	}

}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation
	(FRotator
	(0, 3, 0));
}

