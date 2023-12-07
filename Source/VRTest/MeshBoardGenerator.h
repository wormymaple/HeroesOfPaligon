// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomeModifier.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "MeshBoardGenerator.generated.h"

UCLASS()
class VRTEST_API AMeshBoardGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshBoardGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	UProceduralMeshComponent* MeshComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	float HexWidth;

	UPROPERTY(EditAnywhere)
	int BoardWidth;

	UPROPERTY(EditAnywhere)
	float SeparationDist;

	UPROPERTY(EditAnywhere)
	float HexDepth = 10;

	UPROPERTY(EditAnywhere)
	FLinearColor hexColor;

	UPROPERTY(EditAnywhere)
	ABiomeModifier* ActiveModifier;

	void GenerateBoard();
};
