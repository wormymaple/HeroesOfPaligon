// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	TArray<UMaterial*> TypeMaterials;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ModifyHexes() override;
	virtual void OnConstruction(const FTransform& Transform) override;

};
