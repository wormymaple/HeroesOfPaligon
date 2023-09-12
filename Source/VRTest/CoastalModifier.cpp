// Fill out your copyright notice in the Description page of Project Settings.


#include "CoastalModifier.h"

#include "HexComponent.h"

// Sets default values
ACoastalModifier::ACoastalModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACoastalModifier::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACoastalModifier::OnConstruction(const FTransform& Transform)
{
	GetHexes(GetWorld());
	ModifyHexes();
}

void ACoastalModifier::ModifyHexes()
{
	SetHexMobility(EComponentMobility::Movable);

	for (AActor* hex : Hexes)
	{
		UStaticMeshComponent* hexMesh = hex->GetComponentByClass<UStaticMeshComponent>();
		UHexComponent* hexComponent = hex->GetComponentByClass<UHexComponent>();
		
		FVector hexPos = hex->GetActorLocation();
		FVector2D noiseGenPos = FVector2D(hexPos.X, hexPos.Y) + NoisePos;

		float noise1 =  FMath::Clamp(FMath::PerlinNoise2D(noiseGenPos * NoiseScale) + NoiseOffset, 0, 1 + NoiseOffset);
		float height = noise1 * HeightScale;
		float rockNoise = FMath::PerlinNoise2D(noiseGenPos * RockNoiseScale);

		

		if (rockNoise >= RockNoiseCutoff && noise1 > 0)
		{
			hexMesh->SetMaterial(0, RockMat);
			hex->SetActorLocation(FVector(hexPos.X, hexPos.Y, HexHeight + rockNoise * RockHeight));
			hexComponent->Type = HexType::Rock;
			continue;
		}

		hex->SetActorLocation(FVector(hexPos.X, hexPos.Y, HexHeight + height));
		
		for (int i = 0; i < LowPassFilters.Num(); i += 1)
		{
			if (noise1 <= LowPassFilters[i])
			{
				UMaterial* hexMat = Mats[i];

				if (i == 0)
				{
					hexMesh->SetStaticMesh(nullptr);
				}
				else if (i == SandLayer)
				{
					FRandomStream rand = FRandomStream(noise1 * 255);
					if (static_cast<float>(rand.RandRange(0, 100)) / 100 < RockChance) hexMat = RockMat;
				}
				
				if (hexMat != nullptr) hexMesh->SetMaterial(0, hexMat);
				break;
			}
		}
	}
	
	SetHexMobility(EComponentMobility::Static);
}