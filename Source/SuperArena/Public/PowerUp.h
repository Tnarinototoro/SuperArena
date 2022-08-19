// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaGameState.h"
#include "UsedArenaCharacter.h"
#include "PowerUp.generated.h"

UCLASS()
class SUPERARENA_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PowerUpMesh;
	virtual void PickUp(AUsedArenaCharacter* OwningPlayer);
	virtual void Use(AUsedArenaCharacter* OwningPlayer);
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor
			*
			OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool
			bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
