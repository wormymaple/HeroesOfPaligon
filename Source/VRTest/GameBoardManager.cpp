// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardManager.h"

#include "HexComponent.h"
#include "PawnPiece.h"
#include "SaveHandler.h"
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

	BoardSetup();
	//LootPlacer->PlaceLoot();
}

void AGameBoardManager::BoardSetup()
{
	FGameSave loadedData = SaveHandler->ReadGame(22119);
	
	AActor* closestHex = Hexes[0];
	float closestDist = 9999999999;
	for (AActor* hex : Hexes)
	{
		FVector hexPos = hex->GetActorLocation();
		FVector2D hexPos2D = FVector2D(hexPos.X, hexPos.Y);
		float dist = FVector2D::Distance(hexPos2D, FVector2D::Zero());

		if (dist >= closestDist || hex->GetComponentByClass<UHexComponent>()->Type == HexType::Water) continue;
			
		closestDist = dist;
		closestHex = hex;
	}

	UHexComponent* hexComp = closestHex->GetComponentByClass<UHexComponent>();
	for (int i = 0; i < loadedData.PlayerPackages.Num(); i += 1)
	{
		SpawnPawn(hexComp->AdjacentHexes[i]->GetOwner());
	}
}


// Called every frame
void AGameBoardManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Interacting) return;

	UHexComponent* closestHex = GetClosestHex();
	GhostPawn->GetStaticMeshComponent()->SetWorldLocation(closestHex->GetOwner()->GetActorLocation() + PawnOffset);
}

void AGameBoardManager::SpawnPawn(AActor* Hex)
{
	AActor* newPawn = GetWorld()->SpawnActor(PawnBlueprint->GeneratedClass);
	newPawn->GetComponentByClass<UPawnPiece>()->BoardManager = this;

	newPawn->SetActorLocation(Hex->GetActorLocation() + PawnOffset);
	newPawn->GetComponentByClass<UPawnPiece>()->SetCurrentHex(Hex->GetComponentByClass<UHexComponent>());

	SpawnedPawns.Add(newPawn->GetComponentByClass<UPawnPiece>());
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

AActor* AGameBoardManager::GetMeeple(int index)
{
	if (index > SpawnedPawns.Num()) return nullptr;
	return SpawnedPawns[index]->GetOwner();
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

