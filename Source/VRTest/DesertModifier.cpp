// Fill out your copyright notice in the Description page of Project Settings.


#include "DesertModifier.h"


// Sets default values
ADesertModifier::ADesertModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADesertModifier::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADesertModifier::OnConstruction(const FTransform& Transform)
{
	GetHexes(GetWorld());
	ModifyHexes();
}

void ADesertModifier::ModifyHexes(bool InGame)
{
	for (AActor* hex : Hexes)
	{
		UStaticMeshComponent* hexMesh = hex->GetComponentByClass<UStaticMeshComponent>();
		hexMesh->SetMobility(EComponentMobility::Movable);
		
		FVector hexPos = hex->GetActorLocation();
		FVector2D noisePos = FVector2D(hexPos.X, hexPos.Y) * NoiseScale + NoiseOffset;

		float noise1 = FMath::PerlinNoise2D(noisePos);
		float height = FMath::Clamp(noise1 + NoiseMin, 0, 1 + NoiseMin);

		hex->SetActorLocation(FVector(hexPos.X, hexPos.Y, height * HeightOffset) + FVector::UpVector * StartHeight);

		UMaterial* mat = SandMat;

		if (height <= WaterLowPass)
		{
			mat = WaterMat;
			hexMesh->SetVisibility(false);
		}
		else
		{
			float cactusRandom = FMath::RandRange(0.0, 1.0);
			if (cactusRandom < CactusSpawnRate)
			{
				mat = CactusMat;
			}

			hexMesh->SetVisibility(true);
		}
				
		if (mat != nullptr)
		{
			hexMesh->SetMaterial(0, mat);
		}
		
		hexMesh->SetMobility(EComponentMobility::Static);
	}
}



