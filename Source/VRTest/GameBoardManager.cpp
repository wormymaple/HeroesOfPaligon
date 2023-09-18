// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardManager.h"

#include "HexComponent.h"
#include "PawnPiece.h"
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

	if (!Interacting) return;

	UHexComponent* closestHex = GetClosestHex();
	GhostPawn->GetStaticMeshComponent()->SetWorldLocation(closestHex->GetOwner()->GetActorLocation() + PawnOffset);
}

void AGameBoardManager::SpawnPawn()
{
	AActor* newPawn = GetWorld()->SpawnActor(PawnBlueprint->GeneratedClass);
	newPawn->GetComponentByClass<UPawnPiece>()->BoardManager = this;

	TArray<AActor*> possibleSpawns;
	
	for (AActor* hex : Hexes)
	{
		if (hex->GetComponentByClass<UHexComponent>()->Type != HexType::Water)
		{
			possibleSpawns.Add(hex);
		}	
	}

	AActor* hex = possibleSpawns[FMath::RandRange(0, possibleSpawns.Num() - 1)];
	newPawn->SetActorLocation(hex->GetActorLocation() + PawnOffset);
	newPawn->GetComponentByClass<UPawnPiece>()->SetCurrentHex(hex->GetComponentByClass<UHexComponent>());
}

void AGameBoardManager::PickUpPawn(AActor* InPawn)
{
	if (Interacting) return;
	Interacting = true;
	GhostPawn->GetStaticMeshComponent()->SetVisibility(true);
	
	UPawnPiece* pawnComponent = InPawn->GetComponentByClass<UPawnPiece>();
	InteractingPawn = pawnComponent;
	
	SpawnedHighlights.Empty();
	UHexComponent* pawnHex = pawnComponent->GetCurrentHex();

	for (UHexComponent* adjacentHex : pawnHex->AdjacentHexes)
	{
		FVector hexPos = adjacentHex->GetOwner()->GetActorLocation();

		AActor* newHighlight = GetWorld()->SpawnActor(HighlightMesh->GeneratedClass);
		newHighlight->SetActorLocation(hexPos + PawnOffset);

		SpawnedHighlights.Add(newHighlight);
	}
}

void AGameBoardManager::PlacePawn(AActor* InPawn)
{
	Interacting = false;
	GhostPawn->GetStaticMeshComponent()->SetVisibility(false);
	
	UPawnPiece* pawnComponent = InPawn->GetComponentByClass<UPawnPiece>();
	
	for (AActor* highlight : SpawnedHighlights) GetWorld()->DestroyActor(highlight);
	SpawnedHighlights.Empty();

	UHexComponent* closestHex = GetClosestHex();
	
	UStaticMeshComponent* pawnMesh = InPawn->GetComponentByClass<UStaticMeshComponent>(); 
	pawnMesh->SetWorldLocation(closestHex->GetOwner()->GetActorLocation() + PawnOffset);
	pawnMesh->SetWorldRotation(FRotator::ZeroRotator);
	pawnComponent->SetCurrentHex(closestHex);
}

UHexComponent* AGameBoardManager::GetClosestHex()
{
	UStaticMeshComponent* pawnMesh = InteractingPawn->GetOwner()->GetComponentByClass<UStaticMeshComponent>(); 
	FVector pawnPos = pawnMesh->GetComponentLocation();

	float closestDist = 9999999999;
	UHexComponent* pawnHex = InteractingPawn->GetCurrentHex();
	UHexComponent* closestHex = pawnHex->AdjacentHexes[0];
	for (UHexComponent* adjacentHex : pawnHex->AdjacentHexes)
	{
		float dist = FVector::Dist(adjacentHex->GetOwner()->GetActorLocation(), pawnPos);
		if (dist < closestDist)
		{
			closestHex = adjacentHex;
			closestDist = dist;
		}
	}

	return closestHex;
}

