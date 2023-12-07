// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BiomeModifier.h"
#include "MeshBiomeModifier.h"
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
	FLinearColor GrassColor;
	UPROPERTY(EditAnywhere)
	FLinearColor RockColor;

public:	
	virtual TArray<FLinearColor> GetHexColors(TArray<FVector> hexPositions) override;

};
