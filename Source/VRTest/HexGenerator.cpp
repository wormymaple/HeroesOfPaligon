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
}

// Called every frame
void AHexGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHexGenerator::GenerateHexes(){
	float DepthX = 0.f, DepthY = 0.f;
	float LH = FMath::Sqrt(FMath::Pow(Dist, 2.f) - FMath::Pow(Dist / 2, 2.f));

	for (int i = 0; i < SideWidth + SideWidth - 1; i++){
		int Offset;
		if (i < SideWidth / 2){
			Offset = 1;
		}
		else{
			Offset = SideWidth - (i % SideWidth);
		}

		for (int j = 0; j < SideWidth + Offset; j += 1){
			FActorSpawnParameters Params;
			FVector SpawnPoint = FVector(DepthX + (j * Dist), DepthY, 0);

			AStaticMeshActor* NewHex = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
			NewHex->SetMobility(EComponentMobility::Movable);
			NewHex->SetActorLocation(SpawnPoint);
			NewHex->SetActorScale3D(FVector(0.05f, 0.05f, 0.05f));
			NewHex->GetComponentByClass<UStaticMeshComponent>()->SetStaticMesh(HexMesh);
		}

		DepthX += (Dist / 2) * (Offset == 1 ? -1 : 1);
		DepthY -= LH;
	}
}

