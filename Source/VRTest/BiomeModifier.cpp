// Fill out your copyright notice in the Description page of Project Settings.


#include "BiomeModifier.h"

// Sets default values
ABiomeModifier::ABiomeModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

TArray<FLinearColor> ABiomeModifier::GetHexColors(TArray<FVector> hexPositions)
{
	TArray<FLinearColor> colors;
	colors.Init(FLinearColor::White, hexPositions.Num());
	
	return colors;
}

