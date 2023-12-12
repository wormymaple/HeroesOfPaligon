// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomeModifier.h"
#include "MeshBiomeModifier.h"
#include "MeshBoardGenerator.h"
#include "GameFramework/Actor.h"
#include "PlainsMeshModifier.generated.h"

UCLASS()
class VRTEST_API APlainsMeshModifier : public ABiomeModifier
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlainsMeshModifier();

protected:
	UPROPERTY(EditAnywhere)
	float NoiseScale;

	UPROPERTY(EditAnywhere)
	float NoiseOffset;
	
	UPROPERTY(EditAnywhere)
	float NoiseHeight;

	UPROPERTY(EditAnywhere)
	TArray<float> cutoffStaircase;
	UPROPERTY(EditAnywhere)
	TArray<FLinearColor> staircaseColors;
	UPROPERTY(EditAnywhere)
	TArray<UBlueprint*> staircaseBlueprints;
	UPROPERTY(EditAnywhere)
	TArray<HexType> staircaseTypes;
	
	UPROPERTY(EditAnywhere)
	float DetailChance;

	UPROPERTY(EditAnywhere)
	AMeshBoardGenerator* hexBoard;

public:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual TArray<FVector> GetHexOffsets(TArray<FVector> hexPositions) override;
	virtual TArray<FLinearColor> GetHexColors(TArray<FVector> hexPositions) override;
	virtual TArray<UBlueprint*> GetHexDetails(TArray<FVector> hexPositions) override;
	virtual TArray<HexType> GetHexTypes(TArray<FVector> hexPositions);
	TArray<int> GetCutoffs(TArray<FVector> hexPositions);
};
