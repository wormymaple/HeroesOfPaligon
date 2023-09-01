// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGenerator.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"

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

	SpawnPawn();
}

// Called every frame
void AHexGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AHexGenerator::OnConstruction(const FTransform& Transform)
{
	GenerateHexes();
	ApplyPlains();
}


void AHexGenerator::GenerateHexes(){
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

void AHexGenerator::ApplyPlains()
{
	for (AActor* hex : Hexes)
	{
		UStaticMeshComponent* hexMesh = hex->GetComponentByClass<UStaticMeshComponent>();
		hexMesh->SetMobility(EComponentMobility::Movable);
		
		FVector hexPos = hex->GetActorLocation();
		float height = FMath::Sin(hexPos.X * Wavelength) + FMath::Cos(hexPos.Y * Wavelength);
		height *= OffsetHeight;
		hex->SetActorLocation(FVector(hexPos.X, hexPos.Y, SpawnOffset.Z + height));

		hexMesh->SetMobility(EComponentMobility::Static);
	}
}

void AHexGenerator::SpawnPawn(){
	FActorSpawnParameters Params;
	AActor* NewPawn = GetWorld()->SpawnActor<AActor>(PawnActor->GeneratedClass, Params);

	UStaticMeshComponent* pawnMesh = NewPawn->GetComponentByClass<UStaticMeshComponent>();
	pawnMesh->SetRelativeLocation(SpawnOffset + PawnOffset);
}

void AHexGenerator::PickUpPawn(AActor* InPawnActor)
{
	if (Interacting) return;
	Interacting = true;
	
	UStaticMeshComponent* pawnMesh = InPawnActor->GetComponentByClass<UStaticMeshComponent>();
	AvailableHexes.Empty();
	FVector pawnPos = pawnMesh->GetComponentLocation();
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Black, FString::Printf(TEXT("%f, %f, %f"), pawnPos.X, pawnPos.Y, pawnPos.Z));
	for (AActor* hex : Hexes)
	{
		FVector hexPos = hex->GetActorLocation();
		float dist = FVector2D::Distance(FVector2D(hexPos.X, hexPos.Y), FVector2D(pawnPos.X, pawnPos.Y));

		if (dist < Dist + 5 && dist > 2)
		{
			AvailableHexes.Add(hex);
		}
	}

	SpawnedHiglights.Empty();
	for (AActor* hex : AvailableHexes)
	{
		FVector hexPos = hex->GetActorLocation();
		FActorSpawnParameters Params;

		AActor* newHighlight = GetWorld()->SpawnActor<AActor>(HighlightMesh->GeneratedClass, Params);
		newHighlight->SetActorLocation(hexPos + PawnOffset);
		SpawnedHiglights.Add(newHighlight);
	}
}

void AHexGenerator::DropPawn(AActor* InPawnActor){
	Interacting = false;
	
	UStaticMeshComponent* pawnMesh = InPawnActor->GetComponentByClass<UStaticMeshComponent>();

	FVector pawnPos = pawnMesh->GetComponentLocation();
	
	AActor* closestHex = AvailableHexes[0];
	float closestDist = 10000000;
	for (AActor* hex : AvailableHexes){
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


