// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexComponent.h"
#include "LootPlacer.h"
#include "SaveHandler.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "GameBoardManager.generated.h"

class UPawnPiece;

UCLASS()
class VRTEST_API AGameBoardManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameBoardManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBlueprint* PawnBlueprint;

	UPROPERTY(EditAnywhere)
	FVector PawnOffset;

	UPROPERTY(EditAnywhere)
	UBlueprint* HighlightMesh;

	UPROPERTY(EditAnywhere)
	AStaticMeshActor* GhostPawn;

	UPROPERTY(EditAnywhere)
	ALootPlacer* LootPlacer;

	UPROPERTY(EditAnywhere)
	ASaveHandler* SaveHandler;

private:
	UPROPERTY()
	TArray<AActor*> SpawnedHighlights;
	bool Interacting;
	UPROPERTY()
	UPawnPiece* InteractingPawn;

	AActor* GetClosestHex();

	UPROPERTY()
	TArray<UPawnPiece*> SpawnedPawns;

	UPROPERTY()
	TArray<AActor*> AccessingHexes;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void PickUpPawn(AActor* InPawn);
	UFUNCTION(BlueprintCallable)
	void PlacePawn(AActor* InPawn);
	UFUNCTION(BlueprintCallable)
	AActor* GetMeeple(int index);

	void BoardSetup();
	void SpawnPawn(AActor* Hex, FSaveState Character);
	
	UPROPERTY()
	TArray<AActor*> Hexes;
};
