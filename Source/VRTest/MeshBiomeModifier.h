// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MeshBiomeModifier.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMeshBiomeModifier : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VRTEST_API IMeshBiomeModifier
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual TArray<FLinearColor> GetHexColors(TArray<FVector> hexPositions) = 0;
};
