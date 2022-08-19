// Fill out your copyright notice in the Description page of Project Settings.


#include "TheBall.h"
#include "UsedArenaCharacter.h"

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
	BallMesh->OnComponentHit.AddDynamic(this, &ATheBall::OnHitBegin);
	BallMesh->SetCollisionObjectType(ECC_PhysicsBody);


}

void ATheBall::ServerChangeColour_Implementation(bool TeamOne)
{
	MulticastChangeColour(TeamOne);
}

void ATheBall::MulticastChangeColour_Implementation(bool TeamOne)
{
	if(MaterialInstance)
	{
		if (TeamOne)
		{
			MaterialInstance->SetVectorParameterValue("Colour", GameState->TeamOneColour);
		}
		else
		{
			MaterialInstance->SetVectorParameterValue("Colour", GameState->TeamTwoColour);
		}
	}

}

void ATheBall::OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		if(Cast<ACharacter>(OtherActor))
		{
			if(HasAuthority())
			{
				LastPlayer = Cast<AUsedArenaCharacter>(OtherActor);
				
				if(GameState)
				{
					if
						(GameState->TeamOne.Contains(LastPlayer->GetPlayerState()))
					{
						ServerChangeColour(true);
					}
					else
					{
						ServerChangeColour(false);
					}
				}


			}
		}
	}

}

void ATheBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATheBall, LastPlayer);


}

// Called when the game starts or when spawned
void ATheBall::BeginPlay()
{
	Super::BeginPlay();
	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create
		(MaterialClass, this);
		BallMesh->SetMaterial(0,MaterialInstance);
	}
	GameState=Cast<AArenaGameState>(GetWorld()->GetGameState());

}

// Called every frame
void ATheBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

