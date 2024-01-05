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
		hexComp->GetAdjacentHexes(Dist + 2, &Hexes);
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
}


