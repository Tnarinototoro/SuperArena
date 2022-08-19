// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "ArenaGameState.h"
#include "TheBall.generated.h"


class AUsedArenaCharacter;
UCLASS()
class SUPERARENA_API ATheBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATheBall();
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(Replicated)
	AUsedArenaCharacter* LastPlayer;

	UFUNCTION(Server, Unreliable)
	void ServerChangeColour
	(bool TeamOne);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastChangeColour(bool TeamOne);
	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialClass;
	UMaterialInstanceDynamic* MaterialInstance;
	UFUNCTION()
	void OnHitBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent*
			OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	AArenaGameState* GameState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
