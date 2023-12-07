// Fill out your copyright notice in the Description page of Project Settings.


#include "PlainsMeshModifier.h"

// Sets default values
APlainsMeshModifier::APlainsMeshModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TArray<FLinearColor> APlainsMeshModifier::GetHexColors(TArray<FVector> hexPositions)
{
	TArray<FLinearColor> colors;

	for (FVector hexPos : hexPositions)
	{
		colors.Add(hexPos.Z > 0 ? RockColor : GrassColor);
	}

	return colors;
}

