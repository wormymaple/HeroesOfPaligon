// Fill out your copyright notice in the Description page of Project Settings.

#include "Math/UnrealMathUtility.h"
#include "StatDisc.h"
#include "ProceduralMeshComponent.h"




// Sets default values
AStatDisc::AStatDisc()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStatDisc::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStatDisc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStatDisc::DrawShape(TArray<int> stats)
{
	int statCount = stats.Num();
	float rot = 360 / statCount;

	AActor* newDisc = GetWorld()->SpawnActor(LineSpline->GeneratedClass);
	newDisc->SetActorLocation(GetActorLocation());

	UProceduralMeshComponent* newMesh = newDisc->GetComponentByClass<UProceduralMeshComponent>();

	TArray<FVector> polygonPoints = {FVector::Zero()};
	for (int i = 0; i < statCount; i += 1)
	{
		float statDist = stats[i] / static_cast<float>(15) * DiscRadius + MinRadius;
		FVector2D rotPos = FVector2D(0, statDist).GetRotated(rot * i);

		polygonPoints.Insert(FVector(0, rotPos.X, rotPos.Y), 1);
	}

	for (int i = 0; i < polygonPoints.Num(); i += 1)
	{
		int secondVert = i < polygonPoints.Num() - 1 ? i + 2 : 1;
		newMesh->CreateMeshSection(i, polygonPoints, {0, i + 1, secondVert}, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
	}
}

