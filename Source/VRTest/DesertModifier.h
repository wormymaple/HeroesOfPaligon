// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralModifier.h"
#include "GameFramework/Actor.h"
#include "DesertModifier.generated.h"

UCLASS()
class VRTEST_API ADesertModifier : public AActor, public IProceduralModifier
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADesertModifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float StartHeight;

	UPROPERTY(EditAnywhere)
	FVector2D NoiseOffset;

	UPROPERTY(EditAnywhere)
	float NoiseMin;
	
	UPROPERTY(EditAnywhere)
	float NoiseScale;
	
	UPROPERTY(EditAnywhere)
	float HeightOffset;
	
	UPROPERTY(EditAnywhere)
	float WaterLowPass;

	UPROPERTY(EditAnywhere)
	UMaterial* SandMat;
	
	UPROPERTY(EditAnywhere)
	UMaterial* WaterMat;

	UPROPERTY(EditAnywhere)
	UMaterial* CactusMat;

	UPROPERTY(EditAnywhere)
	float CactusSpawnRate;

public:	
	// Called every frame
	virtual void ModifyHexes(bool InGame = false) override;
	virtual void OnConstruction(const FTransform& Transform) override;

};
