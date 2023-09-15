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
	void GenerateAdjacentHexes();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	void GenerateHexes();

	void ApplyPlains();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlueprint* HexBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Dist;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SideWidth;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnOffset;

	TArray<AActor*> Hexes;


};
