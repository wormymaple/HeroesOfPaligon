// Fill out your copyright notice in the Description page of Project Settings.


#include "PlainsMeshModifier.h"

#include "MeshBoardGenerator.h"

// Sets default values
APlainsMeshModifier::APlainsMeshModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APlainsMeshModifier::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (hexBoard != nullptr) hexBoard->GenerateBoard();
}

TArray<FVector> APlainsMeshModifier::GetHexOffsets(TArray<FVector> hexPositions)
{
	TArray<FVector> positions;

	for (FVector hexPosition : hexPositions)
	{
		FVector noisePos = hexPosition * NoiseScale;
		float noise = FMath::Clamp(FMath::PerlinNoise2D(FVector2D(noisePos.X, noisePos.Y)) + NoiseOffset, 0, 1 + NoiseOffset);

		positions.Add(FVector(hexPosition.X, hexPosition.Y, noise * NoiseHeight));
	}

	return positions;
}

TArray<FLinearColor> APlainsMeshModifier::GetHexColors(TArray<FVector> hexPositions)
{
	TArray<FLinearColor> colors;

	for (int i : GetCutoffs(hexPositions))
	{
		if (i == -1) colors.Add(FLinearColor::Blue);
		else
		{
			if (i <= staircaseColors.Num() - 1)
			{
				colors.Add(staircaseColors[i]);
			}
			else colors.Add(FLinearColor(1, 0, 1, 1)); // PINK
		}
	}

	return colors;
}

TArray<UBlueprint*> APlainsMeshModifier::GetHexDetails(TArray<FVector> hexPositions)
{
	TArray<UBlueprint*> details;

	for (int i : GetCutoffs(hexPositions))
	{
		if (i == -1) details.Add(nullptr);
		else
		{
			if (i <= staircaseBlueprints.Num() - 1)
			{
				if (FMath::RandRange(0.0, 1.0) < DetailChance)
				{
					details.Add(staircaseBlueprints[i]);
				}
				else details.Add(nullptr);
			}
			else details.Add(nullptr);
		}
	}

	return details;
}

TArray<int> APlainsMeshModifier::GetCutoffs(TArray<FVector> hexPositions)
{
	TArray<int> indices;
	
	for (FVector hexPos : hexPositions)
	{
		if (hexPos.Z <= 0)
		{
			indices.Add(-1);
		}
		else
		{
			float adjustedPos = hexPos.Z / NoiseHeight;

			bool foundIndex = false;
			for (int i = 0; i < cutoffStaircase.Num(); i += 1)
			{
				if (adjustedPos < cutoffStaircase[i])
				{
					indices.Add(i);
					foundIndex = true;
					break;
				}	
			}

			if (!foundIndex) indices.Add(-1);
		}
	}

	return indices;
}

