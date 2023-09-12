// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameBoardManager.generated.h"

UCLASS()
class VRTEST_API AGameBoardManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameBoardManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBlueprint* PawnBlueprint;

	UPROPERTY(EditAnywhere)
	FVector PawnOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnPawn();
	
	TArray<AActor*> Hexes;
};
