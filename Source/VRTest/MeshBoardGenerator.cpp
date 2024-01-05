// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshBoardGenerator.h"

#include "HexComponent.h"
#include "StringHelper.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMeshBoardGenerator::AMeshBoardGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMeshBoardGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateAdjacentHexes();
}

// Called every frame
void AMeshBoardGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMeshBoardGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GenerateBoard();
}

void AMeshBoardGenerator::GenerateBoard()
{
	// Destroy details
	TArray<AActor*> details;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("detail"), details);

	for (AActor* detail : details) GetWorld()->DestroyActor(detail);
	
	// Destroy hexes
	TArray<AActor*> hexes;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Hex"), hexes);

	for (AActor* hex : hexes) GetWorld()->DestroyActor(hex);
	
	// Fan
	TArray<FVector> hexVectors;
	for (int i = 0; i < 6; i += 1)
	{
		FVector2D rotVector = FVector2D(1, 0).GetRotated(360.0 / 6.0 * i + 30.0) * HexWidth;
		FVector hexVector = FVector(rotVector.X, rotVector.Y, 0);
		hexVectors.Add(hexVector);
	}
	hexVectors.Add(FVector::ZeroVector);
	// Under fan
	for (int i = 0; i < 6; i += 1)
	{
		hexVectors.Add(hexVectors[i] + FVector::DownVector * HexDepth);
	}

	TArray<int> triangles;
	TArray<FVector> normals;

	for (int i = 0; i < 6; i += 1)
	{
		int vert1 = i;
		int vert2 = i > 0 ? i - 1 : 5;

		triangles += {vert1, vert2, 6};

		normals.Add(FVector::UpVector);
	}
	for (int i = 0; i < 6; i += 1)
	{
		int vert1Top = i;
		int vert2Top = i > 0 ? i - 1 : 5;
		int vert1Bottom = i + 7;
		int vert2Bottom = i > 0 ? i + 6 : 12;

		triangles += {vert1Top, vert2Bottom, vert2Top};
		triangles += {vert1Top, vert1Bottom, vert2Bottom};

		FVector midPoint = (hexVectors[vert1Top], hexVectors[vert2Top]) / 2;
		midPoint.Normalize(0.0001);
		
		normals += {midPoint, midPoint};
	}

	TArray<FVector> hexPositions;
	
	float lh = FMath::Sqrt(FMath::Pow(SeparationDist * 2, 2.f) - FMath::Pow(SeparationDist, 2.f));
	float depthX = -((BoardWidth - 1) / 2.f) * SeparationDist * 2, depthY = (BoardWidth - 1) * lh;

	for (int i = 0; i < BoardWidth + BoardWidth - 1; i++){
		int layerCount, shiftDir;	
		layerCount = i < BoardWidth ? BoardWidth + i : (2 * BoardWidth) - (i % BoardWidth) - 2;
		shiftDir = i < BoardWidth - 1 ? -1 : 1; 

		for (int j = 0; j < layerCount; j += 1){
			FVector spawnPoint = FVector(depthX + (j * SeparationDist * 2), depthY,0);
			hexPositions.Add(spawnPoint);
		}

		depthX += SeparationDist * shiftDir;
		depthY -= lh;
	}


	TArray<FVector> allHexVectors;
	TArray<int> allHexTris;

	TArray<FVector> biomeHexOffsets;
	TArray<FLinearColor> biomeHexColors;
	TArray<UBlueprint*> biomeHexDetails;
	TArray<HexType> biomeHexTypes;
	if (ActiveModifier != nullptr)
	{
		biomeHexOffsets = ActiveModifier->GetHexOffsets(hexPositions);
		biomeHexColors = ActiveModifier->GetHexColors(biomeHexOffsets);
		biomeHexDetails = ActiveModifier->GetHexDetails(biomeHexOffsets);
		biomeHexTypes = ActiveModifier->GetHexTypes(biomeHexOffsets);
	}
	else
	{
		biomeHexOffsets = hexPositions;
		biomeHexColors.Init(FLinearColor::White, hexPositions.Num());
		biomeHexDetails.Init(nullptr, hexPositions.Num());
		biomeHexTypes.Init(HexType::None, hexPositions.Num());
	}
	
	TArray<FLinearColor> allHexColors;

	// Draw hexes
	int hexCount = 0;
	for (int i = 0; i < hexPositions.Num(); i += 1)
	{
		FLinearColor color = biomeHexColors[i];
		
		FVector hexPos = biomeHexOffsets[i];
		if (color == FLinearColor::Blue) hexPos.Z = WaterOffset;
		
		TArray<FVector> newHexVectors;
		TArray<int> newTriangles;
		for (FVector hexVector : hexVectors)
		{
			newHexVectors.Add(hexVector + hexPos);
			allHexColors.Add(color);
		}
		for (int tri : triangles)
		{
			newTriangles.Add(tri + 13 * i);
		}

		allHexVectors += newHexVectors;
		allHexTris += newTriangles;

		GenerateDetail(hexPos, biomeHexDetails[i]);
		SpawnHexAtHexPos(biomeHexOffsets[i], biomeHexTypes[i]);
	
		hexCount += 1;
	}

	MeshComponent = GetComponentByClass<UProceduralMeshComponent>();
	MeshComponent->CreateMeshSection_LinearColor(0, allHexVectors, allHexTris, normals, {}, allHexColors, {}, false);

	GEngine->AddOnScreenDebugMessage(0, 10, FColor::White, StringHelper::IntToString(hexCount));
}

void AMeshBoardGenerator::GenerateDetail(const FVector& HexPos, const UBlueprint* Detail)
{
	if (Detail == nullptr) return;

	FActorSpawnParameters params;
	FVector spawnPos = GetActorLocation() + HexPos;
	FRotator spawnRot = FRotator(FMath::RandRange(-10, 10), FMath::RandRange(0.0, 360.0), FMath::RandRange(-10, 10));
	AActor* newDetail = GetWorld()->SpawnActor(Detail->GeneratedClass, &spawnPos, &spawnRot, params);
	
	newDetail->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

void AMeshBoardGenerator::SpawnHexAtHexPos(const FVector& HexPos, HexType type)
{
	if (HexActor == nullptr) return;
	
	FVector spawnPos = HexPos + GetActorLocation();
	AActor* newHex = GetWorld()->SpawnActor(HexActor->GeneratedClass, &spawnPos, nullptr);

	newHex->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	newHex->GetComponentByClass<UHexComponent>()->Type = type;
}

void AMeshBoardGenerator::GenerateAdjacentHexes()
{
	TArray<AActor*> Hexes;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Hex")), Hexes);
	
	for (AActor* hex : Hexes)
	{	
		UHexComponent* hexComp = hex->GetComponentByClass<UHexComponent>();
		hexComp->GetAdjacentHexes(SeparationDist * 2 + 2, &Hexes);
	}
}

