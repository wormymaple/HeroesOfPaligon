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

	SpawnPawn(closestHex, CreateCharInfoForClass(0));
}

FCharSave AGameBoardManager::CreateCharInfoForClass(int PlayerClass)
{
	FCharSave newChar;
	newChar.CharClass = PlayerClass;
	newChar.Haste = 10;

	return newChar;
}

// Called every frame
void AGameBoardManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Interacting) return;

	AActor* closestHex = GetClosestHex();
	GhostPawn->GetStaticMeshComponent()->SetWorldLocation(closestHex->GetActorLocation() + PawnOffset);
}

void AGameBoardManager::SpawnPawn(AActor* Hex, FCharSave Character)
{
	AActor* newPawn = GetWorld()->SpawnActor(PawnBlueprint->GeneratedClass);
	UPawnPiece* newPawnComp = newPawn->GetComponentByClass<UPawnPiece>(); 
	newPawnComp->BoardManager = this;

	newPawn->SetActorLocation(Hex->GetComponentByClass<UStaticMeshComponent>()->GetComponentLocation() + PawnOffset);
	newPawnComp->SetCurrentHex(Hex->GetComponentByClass<UHexComponent>());
	newPawnComp->CurrentCharacter = Character;
	
	SpawnedPawns.Add(newPawn->GetComponentByClass<UPawnPiece>());
}

void AGameBoardManager::PickUpPawn(AActor* InPawn)
{
	if (Interacting || FinishedMove) return;
	Interacting = true;
	GhostPawn->GetStaticMeshComponent()->SetVisibility(true);
	
	UPawnPiece* pawnComponent = InPawn->GetComponentByClass<UPawnPiece>();
	InteractingPawn = pawnComponent;
	
	SpawnedHighlights.Empty();
	UHexComponent* pawnHex = pawnComponent->GetCurrentHex();

	int haste = (pawnComponent->CurrentCharacter.Haste / 5) + 1;
	TArray<AActor*> accessibleHexes;
	TArray<UHexComponent*> nextHexes = {pawnHex};
	TArray<UHexComponent*> usedHexes = {pawnHex};
	while (haste > 0)
	{
		TArray<UHexComponent*> newHexes;
		for (UHexComponent* hex : nextHexes)
		{
			for (UHexComponent* adjHex : hex->AdjacentHexes)
			{
				if (usedHexes.Contains(adjHex)) continue;
				
				newHexes.Add(adjHex);
				accessibleHexes.Add(adjHex->GetOwner());
				usedHexes.Add(adjHex);
			}
		}
		nextHexes = newHexes;

		haste -= 1;
	}
	
	for (AActor* accessibleHex : accessibleHexes)
	{
		FVector hexPos = accessibleHex->GetComponentByClass<UStaticMeshComponent>()->GetComponentLocation();

		AActor* newHighlight = GetWorld()->SpawnActor(HighlightMesh->GeneratedClass);
		newHighlight->SetActorLocation(hexPos + PawnOffset);

		SpawnedHighlights.Add(newHighlight);
	}

	AccessingHexes = accessibleHexes;
}

void AGameBoardManager::PlacePawn(AActor* InPawn)
{
	UPawnPiece* pawnComponent = InPawn->GetComponentByClass<UPawnPiece>();
	UStaticMeshComponent* pawnMesh = InPawn->GetComponentByClass<UStaticMeshComponent>(); 

	if (FinishedMove)
	{
		pawnMesh->SetWorldLocation(pawnComponent->GetCurrentHex()->GetOwner()->GetComponentByClass<UStaticMeshComponent>()->GetComponentLocation() + PawnOffset);
		pawnMesh->SetWorldRotation(FRotator::ZeroRotator);
		return;
	}
	
	Interacting = false;
	GhostPawn->GetStaticMeshComponent()->SetVisibility(false);
	
	for (AActor* highlight : SpawnedHighlights) GetWorld()->DestroyActor(highlight);
	SpawnedHighlights.Empty();

	AActor* closestHex = GetClosestHex();
	
	pawnMesh->SetWorldLocation(closestHex->GetActorLocation() + PawnOffset);
	pawnMesh->SetWorldRotation(FRotator::ZeroRotator);
	pawnComponent->SetCurrentHex(closestHex->GetComponentByClass<UHexComponent>());

	FinishedMove = true;
}

AActor* AGameBoardManager::GetMeeple(int index)
{
	if (index > SpawnedPawns.Num() - 1) return nullptr;
	return SpawnedPawns[index]->GetOwner();
}

void AGameBoardManager::EndMove()
{
	FinishedMove = false;
}

void AGameBoardManager::EndGame()
{
	for (UPawnPiece* Pawn : SpawnedPawns)
	{
		GetWorld()->DestroyActor(Pawn->GetOwner());
	}
}

AActor* AGameBoardManager::GetClosestHex()
{
	UStaticMeshComponent* pawnMesh = InteractingPawn->GetOwner()->GetComponentByClass<UStaticMeshComponent>(); 
	FVector pawnPos = pawnMesh->GetComponentLocation();

	float closestDist = 9999999999;
	AActor* closestHex = AccessingHexes[0];
	for (AActor* accessingHex : AccessingHexes)
	{
		float dist = FVector::Dist(accessingHex->GetActorLocation(), pawnPos);
		if (dist < closestDist)
		{
			closestHex = accessingHex;
			closestDist = dist;
		}
	}

	return closestHex;
}

