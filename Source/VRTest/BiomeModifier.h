// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BiomeModifier.generated.h"

UCLASS()
class VRTEST_API ABiomeModifier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABiomeModifier();

protected:

public:
	virtual TArray<FVector> GetHexOffsets(TArray<FVector> hexPositions);
	virtual TArray<FLinearColor> GetHexColors(TArray<FVector> hexPositions);
	virtual TArray<UBlueprint*> GetHexDetails(TArray<FVector> hexPositions);
};
