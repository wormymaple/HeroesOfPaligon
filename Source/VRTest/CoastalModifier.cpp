// Fill out your copyright notice in the Description page of Project Settings.


#include "CoastalModifier.h"

#include "HexComponent.h"
#include "Kismet/GameplayStatics.h"

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

void ACoastalModifier::ModifyHexes(bool InGame)
{
	SetHexMobility(EComponentMobility::Movable);

	TArray<AActor*> details;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("detail")), details);
	for (AActor* detail : details) GetWorld()->DestroyActor(detail);

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
			hexMesh->SetVisibility(true);
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
				HexType hexType = HexType::None;
				if (i < Types.Num()) hexType = Types[i];

				hexMesh->SetVisibility(i != 0); // Is water?
				
				if (i == SandLayer) // Is sand?
				{
					FRandomStream rand = FRandomStream(noise1 * 255);
					if (static_cast<float>(rand.RandRange(0, 100)) / 100 < RockChance) // Generate rocks
					{
						hexMat = RockMat;
						hexType = HexType::Rock;
					}
				}
				
				if (hexMat != nullptr) hexMesh->SetMaterial(0, hexMat);
				hexComponent->Type = hexType;
				break;
			}
		}

		HexType currentHexType = hexComponent->Type;

		if (currentHexType == HexType::Rock && RockDetail != nullptr)
		{
			int randRot = FMath::RandRange(0, 360);
			AActor* detail = GetWorld()->SpawnActor(RockDetail->GeneratedClass);
			detail->SetActorLocation(hexPos + DetailOffset);
			detail->SetActorRotation(FRotator(0, randRot, 0));
			detail->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
	
	SetHexMobility(EComponentMobility::Static);
}