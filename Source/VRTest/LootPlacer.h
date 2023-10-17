// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexComponent.h"
#include "GameFramework/Actor.h"
#include "LootPlacer.generated.h"

UCLASS()
class VRTEST_API ALootPlacer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootPlacer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBlueprint* GoldPiece;
	
	UPROPERTY(EditAnywhere)
	FVector GoldOffset;

	UPROPERTY(EditAnywhere)
	TArray<HexType> LootHexes;

	UPROPERTY(EditAnywhere)
	float GoldChance;

	UPROPERTY(EditAnywhere)
	UBlueprint* Enemy;

	UPROPERTY(EditAnywhere)
	FVector EnemyOffset;

	UPROPERTY(EditAnywhere)
	float CircleRadius;

	UPROPERTY(EditAnywhere)
	int BlobCount;

	UPROPERTY(EditAnywhere)
	int EnemySpawnCount;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void PlaceLoot();
	void PlaceEnemies();

	void SpawnEnemy(FVector Pos);
	
	TArray<AActor*> GetHexes();
};
