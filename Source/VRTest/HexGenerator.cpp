// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGenerator.h"
#include "Engine/StaticMeshActor.h"

// Sets default values
AHexGenerator::AHexGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHexGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateHexes();
	SpawnPawn();
}

// Called every frame
void AHexGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AHexGenerator::GenerateHexes(){
	float lh = FMath::Sqrt(FMath::Pow(Dist, 2.f) - FMath::Pow(Dist / 2, 2.f));
	float depthX = -((SideWidth - 1) / 2.f) * Dist, depthY = (SideWidth - 1) * lh;

	for (int i = 0; i < SideWidth + SideWidth - 1; i++){
		int layerCount, shiftDir;	
		layerCount = i < SideWidth ? SideWidth + i : (2 * SideWidth) - (i % SideWidth) - 2;
		shiftDir = i < SideWidth - 1 ? -1 : 1; 

		for (int j = 0; j < layerCount; j += 1){
			FActorSpawnParameters Params;
			FVector SpawnPoint = FVector(depthX + (j * Dist), depthY, SpawnOffset.Z);

			AActor* NewHex = GetWorld()->SpawnActor<AActor>(HexBlueprint->GeneratedClass, Params);
			NewHex->SetActorLocation(SpawnPoint);

			Hexes.Add(NewHex);
		}

		depthX += (Dist / 2) * shiftDir;
		depthY -= lh;
	}
}

void AHexGenerator::SpawnPawn(){
	FActorSpawnParameters Params;
	AActor* NewPawn = GetWorld()->SpawnActor<AActor>(PawnActor->GeneratedClass, Params);

	UStaticMeshComponent* pawnMesh = NewPawn->GetComponentByClass<UStaticMeshComponent>();
	pawnMesh->SetRelativeLocation(FVector(0, 100, 0));
}

void AHexGenerator::PickUpPawn(AActor* InPawnActor){
	UStaticMeshComponent* pawnMesh = InPawnActor->GetComponentByClass<UStaticMeshComponent>();
	TArray<AActor*> surroundingHexes;
	FVector pawnPos = pawnMesh->GetComponentLocation();
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("%f, %f, %f"), pawnPos.X, pawnPos.Y, pawnPos.Z));
	for (AActor* hex : Hexes)
	{
		FVector hexPos = hex->GetActorLocation();
		float dist = FVector2D::Distance(FVector2D(hexPos.X, hexPos.Y), FVector2D(pawnPos.X, pawnPos.Y));

		if (dist < Dist + 5 && dist > 2)
		{
			surroundingHexes.Add(hex);
		}
	}

	SpawnedHiglights.Empty();
	for (AActor* hex : surroundingHexes)
	{
		FVector hexPos = hex->GetActorLocation();
		FActorSpawnParameters Params;

		AActor* newHighlight = GetWorld()->SpawnActor<AActor>(HighlightMesh->GeneratedClass, Params);
		newHighlight->SetActorLocation(hexPos + PawnOffset);
		SpawnedHiglights.Add(newHighlight);
	}
}

void AHexGenerator::DropPawn(AActor* InPawnActor){
	UStaticMeshComponent* pawnMesh = InPawnActor->GetComponentByClass<UStaticMeshComponent>();

	FVector pawnPos = pawnMesh->GetComponentLocation();
	
	AActor* closestHex = Hexes[0];
	float closestDist = FVector::Dist(pawnPos, closestHex->GetActorLocation());
	for (AActor* hex : Hexes){
		float dist = FVector::Dist(pawnPos, hex->GetActorLocation());
		if (dist < closestDist){
			closestHex = hex;
			closestDist = dist;
		}
	}

	for (AActor* highlight : SpawnedHiglights)
	{
		GetWorld()->DestroyActor(highlight);
	}

	pawnMesh->SetWorldLocation(closestHex->GetActorLocation() + PawnOffset);
	pawnMesh->SetWorldRotation(FRotator(0, 0, 0));
}

void AHexGenerator::PlacePawnRandomly(AActor* InPawnActor)
{
	UStaticMeshComponent* pawnMesh = InPawnActor->GetComponentByClass<UStaticMeshComponent>();

	pawnMesh->SetRelativeLocation(Hexes[FMath::RandRange(0, Hexes.Num() - 1)]->GetActorLocation() + PawnOffset);
	PickUpPawn(InPawnActor);
}


