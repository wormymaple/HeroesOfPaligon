// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexComponent.h"
#include "PawnPiece.h"
#include "ProceduralModifier.h"
#include "GameFramework/Actor.h"
#include "CoastalModifier.generated.h"

UCLASS()
class VRTEST_API ACoastalModifier : public AActor, public IProceduralModifier
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoastalModifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void ModifyHexes() override;

	UPROPERTY(EditAnywhere)
	float HexHeight;
	
	UPROPERTY(EditAnywhere)
	FVector2D NoisePos;
	
	UPROPERTY(EditAnywhere)
	float NoiseScale;
	UPROPERTY(EditAnywhere)
	float NoiseOffset;
	UPROPERTY(EditAnywhere)
	float HeightScale;

	UPROPERTY(EditAnywhere)
	int SandLayer;
	UPROPERTY(EditAnywhere)
	float RockChance;
	
	UPROPERTY(EditAnywhere)
	UMaterial* RockMat;
	UPROPERTY(EditAnywhere)
	float RockNoiseScale;
	UPROPERTY(EditAnywhere)
	float RockNoiseCutoff;
	UPROPERTY(EditAnywhere)
	float RockHeight;

	UPROPERTY(EditAnywhere)
	TArray<float> LowPassFilters;

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> Mats;
	UPROPERTY(EditAnywhere)
	TArray<HexType> Types;
};
