// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "HexGenerator.generated.h"

UCLASS()
class VRTEST_API AHexGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void GenerateHexes();
	void SpawnPawn();

	void ApplyPlains();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlueprint* HexBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Dist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SideWidth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlueprint* PawnActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlueprint* HighlightMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PawnOffset;

	TArray<AActor*> Hexes;
	TArray<AActor*> SpawnedHiglights;
	TArray<AActor*> AvailableHexes;
	bool Interacting;

	UPROPERTY(EditAnywhere)
	float OffsetHeight;
	UPROPERTY(EditAnywhere)
	float Wavelength;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Hex")
	void PickUpPawn(AActor* InPawnActor);

	UFUNCTION(BlueprintCallable, Category="Hex")
	void DropPawn(AActor* InPawnActor);
	
	UFUNCTION(BlueprintCallable, Category="Hex")
	void PlacePawnRandomly(AActor* InPawnActor);

};
