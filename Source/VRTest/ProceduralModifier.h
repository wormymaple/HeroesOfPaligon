// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LootPlacer.h"
#include "UObject/Interface.h"
#include "ProceduralModifier.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProceduralModifier : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VRTEST_API IProceduralModifier
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ModifyHexes() = 0;
	void GetHexes(UObject* World);
	void SetHexMobility(EComponentMobility::Type MobilityType);
	ALootPlacer* GetLoot(UObject* World);
	void DestroyDetails(UObject* World);

	TArray<AActor*> Hexes;
};
