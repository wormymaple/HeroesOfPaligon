// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardManager.h"

#include "HexComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameBoardManager::AGameBoardManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameBoardManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Hex"), Hexes);

	SpawnPawn();
}

// Called every frame
void AGameBoardManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGameBoardManager::SpawnPawn()
{
	AActor* newPawn = GetWorld()->SpawnActor(PawnBlueprint->GeneratedClass);
	for (AActor* hex : Hexes)
	{
		if (hex->GetComponentByClass<UHexComponent>()->Type != HexType::Rock) continue;
		newPawn->SetActorLocation(hex->GetActorLocation() + PawnOffset);
		break;
	};
}


