// Fill out your copyright notice in the Description page of Project Settings.


#include "LootPlacer.h"

#include "StringHelper.h"
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
	TArray<AActor*> UsedHexes;

	float rot = 360.0 / BlobCount;
	for (int i = 0; i < BlobCount; i += 1)
	{
		FVector2D blobPos = CircleRadius * FVector2D(1, 0).GetRotated(rot * i);

		AActor* closestHex = Hexes[0];
		float closestDist = 9999999999;
		for (AActor* hex : Hexes)
		{
			FVector hexPos = hex->GetActorLocation();
			FVector2D hexPos2D = FVector2D(hexPos.X, hexPos.Y);
			float dist = FVector2D::Distance(hexPos2D, blobPos);

			if (dist >= closestDist || hex->GetComponentByClass<UHexComponent>()->Type == HexType::Water) continue;
			
			closestDist = dist;
			closestHex = hex;
		}

		SpawnEnemy(closestHex->GetActorLocation() + EnemyOffset);

		UHexComponent* hexComp = closestHex->GetComponentByClass<UHexComponent>();
		hexComp->GetAdjacentHexes(10, Hexes);
		int hexCount = hexComp->AdjacentHexes.Num();
		int spawnCount = EnemySpawnCount > hexCount ? hexCount : EnemySpawnCount;
		for (int j = 0; j < spawnCount; j += 1)
		{
			SpawnEnemy(hexComp->AdjacentHexes[j]->GetOwner()->GetActorLocation() + EnemyOffset);
		}
	}
}

TArray<AActor*> ALootPlacer::GetHexes()
{
	TArray<AActor*> Hexes;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Hex")), Hexes);
	return Hexes;
}

void ALootPlacer::SpawnEnemy(FVector Pos)
{
	AActor* newEnemy = GetWorld()->SpawnActor(Enemy->GeneratedClass);
	newEnemy->SetActorLocation(Pos);
	newEnemy->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

