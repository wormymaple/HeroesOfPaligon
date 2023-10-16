// Fill out your copyright notice in the Description page of Project Settings.


#include "LootPlacer.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ALootPlacer::ALootPlacer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALootPlacer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootPlacer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALootPlacer::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	PlaceEnemies();
}

void ALootPlacer::PlaceLoot()
{
	TArray<AActor*> Hexes = GetHexes();

	for (AActor* hex : Hexes)
	{
		UHexComponent* hexComp = hex->GetComponentByClass<UHexComponent>();
		if (!LootHexes.Contains(hexComp->Type)) continue;
		
		float goldChance = FMath::RandRange(0.0, 1.0);
		if (goldChance > GoldChance) continue;
		
		AActor* newGold = GetWorld()->SpawnActor(GoldPiece->GeneratedClass);
		newGold->SetActorLocation(hex->GetActorLocation() + GoldOffset);

		hexComp->AddLoot(newGold);
	}
}

void ALootPlacer::PlaceEnemies()
{
	// Destroy all enemies
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Enemy")), Enemies);
	for (AActor* enemy : Enemies) GetWorld()->DestroyActor(enemy);

	TArray<AActor*> Hexes = GetHexes(); // Get hexes in world
	for (AActor* hex : Hexes)
	{
		FVector hexWorldPos = hex->GetActorLocation();
		FVector2D hexPos = FVector2D(hexWorldPos);

		float noise = FMath::PerlinNoise2D((hexPos + NoiseOffset) * NoiseScale);

		if (noise < NoiseHighCutoff) continue;
		
		AActor* newEnemy = GetWorld()->SpawnActor(Enemy->GeneratedClass);
		newEnemy->SetActorLocation(hex->GetActorLocation() + EnemyOffset);
		newEnemy->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

TArray<AActor*> ALootPlacer::GetHexes()
{
	TArray<AActor*> Hexes;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Hex")), Hexes);
	return Hexes;
}

