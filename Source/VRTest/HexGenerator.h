// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HexGenerator.generated.h"

UCLASS()
class VRTEST_API AHexGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void GenerateHexes();

	void ApplyPlains();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlueprint* HexBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Dist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SideWidth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnOffset;

	TArray<AActor*> Hexes;

	UPROPERTY(EditAnywhere)
	FVector2D NoisePos;
	
	UPROPERTY(EditAnywhere)
	float OffsetHeight;
	
	UPROPERTY(EditAnywhere)
	float NoiseOffset;
	UPROPERTY(EditAnywhere)
	float NoiseScale;

	UPROPERTY(EditAnywhere)
	float HighPassCutoff;
	UPROPERTY(EditAnywhere)
	float LowPassCutoff;
	
	UPROPERTY(EditAnywhere)
	float SecondNoiseScale;

	int OldSideWidth;
	float OldDist;

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> TypeMaterials;


};
