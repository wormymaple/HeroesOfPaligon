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

			AStaticMeshActor* NewHex = GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass());
			NewHex->SetMobility(EComponentMobility::Movable);
			NewHex->SetActorLocation(SpawnPoint);
			NewHex->SetActorScale3D(FVector(0.05f, 0.05f, 0.05f));
			UStaticMeshComponent* HexMeshComp = NewHex->GetComponentByClass<UStaticMeshComponent>();
			HexMeshComp->SetStaticMesh(HexMesh);
			HexMeshComp->SetMaterial(0, HexMat);
		}

		depthX += (Dist / 2) * shiftDir;
		depthY -= lh;
	}
}

void AHexGenerator::SpawnPawn(){
	FActorSpawnParameters Params;
	AActor* NewPawn = GetWorld()->SpawnActor<AActor>(PawnActor->GeneratedClass, Params);

	UStaticMeshComponent* PawnMesh = NewPawn->GetComponentByClass<UStaticMeshComponent>();
	PawnMesh->GetOwner()->SetActorLocation(SpawnOffset + PawnOffset);

	PawnMesh->SetSimulatePhysics(true);
}

void AHexGenerator::PickUpPawn(AActor* InPawnActor){
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Green, TEXT("Pawn Picked Up!"));
}

void AHexGenerator::DropPawn(AActor* InPawnActor){
	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Blue, TEXT("Pawn Dropped!"));
}

