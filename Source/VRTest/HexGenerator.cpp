// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGenerator.h"

#include "HexComponent.h"
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
	GenerateAdjacentHexes();
}

void AHexGenerator::OnConstruction(const FTransform& Transform)
{
	GenerateHexes();
}

void AHexGenerator::GenerateAdjacentHexes()
{
	for (AActor* hex : Hexes)
	{	
		UHexComponent* hexComp = hex->GetComponentByClass<UHexComponent>();
		hexComp->GetAdjacentHexes(Dist + 2, Hexes);
	}
}

void AHexGenerator::GenerateHexes()
{
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Hex")), Hexes);

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
}


