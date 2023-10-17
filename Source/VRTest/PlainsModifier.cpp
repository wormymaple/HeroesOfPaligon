// Fill out your copyright notice in the Description page of Project Settings.


#include "PlainsModifier.h"

#include "Interfaces/IPluginManager.h"

// Sets default values
APlainsModifier::APlainsModifier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlainsModifier::BeginPlay()
{
	Super::BeginPlay();

}

void APlainsModifier::OnConstruction(const FTransform& Transform)
{
	GetHexes(GetWorld());
	ModifyHexes();
}


// Called every frame
void APlainsModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlainsModifier::ModifyHexes()
{
	for (AActor* hex : Hexes)
	{
		UStaticMeshComponent* hexMesh = hex->GetComponentByClass<UStaticMeshComponent>();
		hexMesh->SetMobility(EComponentMobility::Movable);
		
		FVector hexPos = hex->GetActorLocation();
		FVector2D GenNoisePos = FVector2D(hexPos.X, hexPos.Y) + NoisePos;
		
		float noise1 =  FMath::Clamp(static_cast<double>(FMath::PerlinNoise2D(GenNoisePos * NoiseScale * ScaleMultiplier)) + NoiseOffset, 0.0, 1.0 + NoiseOffset); 
		float height = noise1 * OffsetHeight;
		hex->SetActorLocation(FVector(hexPos.X, hexPos.Y, StartHeight + height));

		float noise2 = FMath::PerlinNoise2D(GenNoisePos * SecondNoiseScale * ScaleMultiplier);

		UHexComponent* hexComp = hex->GetComponentByClass<UHexComponent>();

		if (TypeMaterials.Num() != 0)
		{
			UMaterial* TileMat;
			if (noise1 <= LowPassCutoff)
			{
				TileMat = TypeMaterials[0];
				hexComp->Type = HexType::Water;
			}
			else if (noise1 >= HighPassCutoff)
			{
				TileMat = TypeMaterials[TypeMaterials.Num() - 1];
				hexComp->Type = HexType::Rock;
			}
			else
			{
				int matIndex = ((noise2 + 1) / 2) * (TypeMaterials.Num() - 2);
				TileMat = TypeMaterials[matIndex + 1];

				hexComp->Type = HexType::Ground;
			}
			hexMesh->SetVisibility(noise1 > LowPassCutoff);
			
			if (TileMat != nullptr)
			{
				hexMesh->SetMaterial(0, TileMat); 
			}
		}

		hexMesh->SetMobility(EComponentMobility::Static);
	}

	if (LootPlacer != nullptr) LootPlacer->PlaceEnemies();
}


