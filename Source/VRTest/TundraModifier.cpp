// Fill out your copyright notice in the Description page of Project Settings.


#include "TundraModifier.h"

// Sets default values
ATundraModifier::ATundraModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it. 
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATundraModifier::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATundraModifier::OnConstruction(const FTransform& Transform)
{
	GetHexes(GetWorld());
	ModifyHexes();
}


void ATundraModifier::ModifyHexes(bool InGame)
{
	SetHexMobility(EComponentMobility::Movable);

	for (AActor* hex : Hexes)
	{
		UStaticMeshComponent* hexMesh = hex->GetComponentByClass<UStaticMeshComponent>();
		
		FVector hexPos = hex->GetActorLocation();
		FVector2D noiseGenPos = FVector2D(hexPos.X, hexPos.Y) + NoisePos;

		float noise1 =  FMath::Clamp(FMath::PerlinNoise2D(noiseGenPos * NoiseScale) + NoiseOffset, 0, 1 + NoiseOffset);
		float height = noise1 * HeightScale;

		hex->SetActorLocation(FVector(hexPos.X, hexPos.Y, HexHeight + height));

		if (noise1 <= WaterLowPass)
		{
			hexMesh->SetMaterial(0, WaterMat);
		}
		else if (noise1 >= RockHighPass)
		{
			hexMesh->SetMaterial(0, RockMat);
		}
		else hexMesh->SetMaterial(0, SnowMat);
		hexMesh->SetVisibility(noise1 > WaterLowPass);
	}
	
	SetHexMobility(EComponentMobility::Static);
}


