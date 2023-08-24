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
	void GenerateHexes();

	UPROPERTY(EditAnywhere)
	UStaticMesh* HexMesh;
	UPROPERTY(EditAnywhere)
	UMaterial* HexMat;

	UPROPERTY(EditAnywhere)
	float Dist;
	UPROPERTY(EditAnywhere)
	int SideWidth;

	UPROPERTY(EditAnywhere)
	FVector SpawnOffset;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
