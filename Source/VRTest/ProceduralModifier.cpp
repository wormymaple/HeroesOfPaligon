// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralModifier.h"

#include "Kismet/GameplayStatics.h"

// Add default functionality here for any IProceduralModifier functions that are not pure virtual.
void IProceduralModifier::GetHexes(UObject* World)
{
	UGameplayStatics::GetAllActorsWithTag(World, FName(TEXT("Hex")), Hexes);
}

void IProceduralModifier::SetHexMobility(EComponentMobility::Type MobilityType)
{
	for (AActor* hex : Hexes)
	{
		hex->GetComponentByClass<UStaticMeshComponent>()->SetMobility(MobilityType);
	}
}

ALootPlacer* IProceduralModifier::GetLoot(UObject* World)
{
	TArray<AActor*> LootPlacers;
	UGameplayStatics::GetAllActorsOfClass(World, ALootPlacer::StaticClass(), LootPlacers);

	return nullptr;
}

