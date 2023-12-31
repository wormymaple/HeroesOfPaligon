// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexComponent.h"
#include "ProceduralModifier.h"
#include "GameFramework/Actor.h"
#include "PlainsModifier.generated.h"

UCLASS()
class VRTEST_API APlainsModifier : public AActor, public IProceduralModifier
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlainsModifier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PreInitializeComponents() override;
	
	UPROPERTY(EditAnywhere)
	FVector2D NoisePos;
	
	UPROPERTY(EditAnywhere)
	float StartHeight;
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

	UPROPERTY(EditAnywhere)
	float ScaleMultiplier = 1;
	
	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> TypeMaterials;

	UPROPERTY(EditAnywhere)
	ALootPlacer* LootPlacer;

	UPROPERTY(EditAnywhere)
	UBlueprint* PineTree;
	UPROPERTY(EditAnywhere)
	UBlueprint* PalmTree;
	UPROPERTY(EditAnywhere)
	UBlueprint* RockDetail;

	UPROPERTY(EditAnywhere)
	float PineTreeChance;

	UPROPERTY(EditAnywhere)
	FVector DetailOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ModifyHexes(bool InGame = false) override;
	virtual void OnConstruction(const FTransform& Transform) override;

};
