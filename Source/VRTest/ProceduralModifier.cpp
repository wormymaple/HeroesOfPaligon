// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralModifier.h"

#include "Kismet/GameplayStatics.h"

// Add default functionality here for any IProceduralModifier functions that are not pure virtual.
void IProceduralModifier::GetHexes(UObject* World)
{
	UGameplayStatics::GetAllActorsWithTag(World, FName(TEXT("Hex")), Hexes);
}
