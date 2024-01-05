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
}

void ALootPlacer::PlaceEnemies()
{
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

