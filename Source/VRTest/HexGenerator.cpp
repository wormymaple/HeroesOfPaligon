// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGenerator.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHexGenerator::AHexGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AHexGenerator::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Hex")), Hexes);
}

void AHexGenerator::OnConstruction(const FTransform& Transform)
{
	GenerateHexes();
}


void AHexGenerator::GenerateHexes()
{
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Hex")), Hexes);
	if (OldSideWidth == SideWidth && OldDist == Dist) return;

	// Destroy old hexes
	for (AActor* hex : Hexes)
	{
		GetWorld()->DestroyActor(hex);
	}
	
	Hexes.Empty();
	
	float lh = FMath::Sqrt(FMath::Pow(Dist, 2.f) - FMath::Pow(Dist / 2, 2.f));
	FVector pos = GetActorLocation();
	float depthX = -((SideWidth - 1) / 2.f) * Dist + pos.X, depthY = (SideWidth - 1) * lh + pos.Y;

	int hexCount = 0;
	for (int i = 0; i < SideWidth + SideWidth - 1; i++){
		int layerCount, shiftDir;	
		layerCount = i < SideWidth ? SideWidth + i : (2 * SideWidth) - (i % SideWidth) - 2;
		shiftDir = i < SideWidth - 1 ? -1 : 1; 

		for (int j = 0; j < layerCount; j += 1){
			FActorSpawnParameters Params;
			FVector SpawnPoint = FVector(depthX + (j * Dist), depthY, SpawnOffset.Z);

			AActor* NewHex = GetWorld()->SpawnActor<AActor>(HexBlueprint->GeneratedClass, Params);
			NewHex->SetActorLabel(FString::Printf(TEXT("Hex%i"), hexCount));
			NewHex->SetActorLocation(SpawnPoint);
			NewHex->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			NewHex->GetComponentByClass<UStaticMeshComponent>()->SetMobility(EComponentMobility::Static);

			Hexes.Add(NewHex);
			hexCount += 1;
		}

		depthX += (Dist / 2) * shiftDir;
		depthY -= lh;
	}

	OldSideWidth = SideWidth;
	OldDist = Dist;
}

void AHexGenerator::ApplyPlains()
{
	for (AActor* hex : Hexes)
	{
		UStaticMeshComponent* hexMesh = hex->GetComponentByClass<UStaticMeshComponent>();
		hexMesh->SetMobility(EComponentMobility::Movable);
		
		FVector hexPos = hex->GetActorLocation();
		FVector2D GenNoisePos = FVector2D(hexPos.X, hexPos.Y) + NoisePos;
		
		float noise1 =  FMath::Clamp(FMath::PerlinNoise2D(GenNoisePos * NoiseScale) + NoiseOffset, 0, 1 + NoiseOffset); 
		float height = noise1 * OffsetHeight;
		hex->SetActorLocation(FVector(hexPos.X, hexPos.Y, SpawnOffset.Z + height));

		float noise2 = FMath::PerlinNoise2D(GenNoisePos * SecondNoiseScale);

		if (TypeMaterials.Num() != 0)
		{
			UMaterial* TileMat;
			if (noise1 <= LowPassCutoff)
			{
				TileMat = TypeMaterials[0];
			}
			else if (noise1 >= HighPassCutoff)
			{
				TileMat = TypeMaterials[TypeMaterials.Num() - 1];
			}
			else
			{
				int matIndex = ((noise2 + 1) / 2) * (TypeMaterials.Num() - 2);
				TileMat = TypeMaterials[matIndex + 1];
			}
			
			if (TileMat != nullptr)
			{
				hexMesh->SetMaterial(0, TileMat);
			}
		}

		hexMesh->SetMobility(EComponentMobility::Static);
	}
}


