// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshBoardGenerator.h"
#include "StringHelper.h"

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
			FVector spawnPoint = FVector(depthX + (j * SeparationDist * 2), depthY, FMath::Sin(static_cast<float>(depthX + j + depthY)) * 4);
			hexPositions.Add(spawnPoint);
		}

		depthX += SeparationDist * shiftDir;
		depthY -= lh;
	}


	TArray<FVector> allHexVectors;
	TArray<int> allHexTris;

	TArray<FLinearColor> biomeHexColors;
	if (ActiveModifier != nullptr)
	{
		biomeHexColors = ActiveModifier->GetHexColors(hexPositions);
	}
	else biomeHexColors.Init(FLinearColor::White, hexPositions.Num());
	
	TArray<FLinearColor> allHexColors;
	
	int hexCount = 0;
	for (int i = 0; i < hexPositions.Num(); i += 1)
	{
		FVector point = hexPositions[i];
		
		TArray<FVector> newHexVectors;
		TArray<int> newTriangles;
		for (FVector hexVector : hexVectors)
		{
			newHexVectors.Add(hexVector + point);
			allHexColors.Add(biomeHexColors[i]);
		}
		for (int tri : triangles)
		{
			newTriangles.Add(tri + 13 * i);
		}

		allHexVectors += newHexVectors;
		allHexTris += newTriangles;
	
		hexCount += 1;
	}

	MeshComponent = GetComponentByClass<UProceduralMeshComponent>();
	MeshComponent->CreateMeshSection_LinearColor(0, allHexVectors, allHexTris, normals, {}, allHexColors, {}, false);

	GEngine->AddOnScreenDebugMessage(0, 10, FColor::White, StringHelper::IntToString(hexCount));
}

