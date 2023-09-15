// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HexComponent.h"
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

private:
	TArray<AActor*> SpawnedHighlights;
	bool Interacting;
	UPawnPiece* InteractingPawn;

	UHexComponent* GetClosestHex();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void PickUpPawn(AActor* InPawn);
	UFUNCTION(BlueprintCallable)
	void PlacePawn(AActor* InPawn);

	void SpawnPawn();
	
	TArray<AActor*> Hexes;
};
