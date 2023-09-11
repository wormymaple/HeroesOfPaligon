// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralModifier.h"
#include "GameFramework/Actor.h"
#include "TundraModifier.generated.h"

UCLASS()
class VRTEST_API ATundraModifier : public AActor, public IProceduralModifier
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATundraModifier();

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
	float WaterLowPass;
	UPROPERTY(EditAnywhere)
	float RockHighPass;
	
	UPROPERTY(EditAnywhere)
	UMaterial* WaterMat;
	UPROPERTY(EditAnywhere)
	UMaterial* SnowMat;
	UPROPERTY(EditAnywhere)
	UMaterial* RockMat;

};
