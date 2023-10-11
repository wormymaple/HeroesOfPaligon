// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StatDisc.generated.h"

UCLASS()
class VRTEST_API AStatDisc : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatDisc();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UBlueprint* LineSpline;

	UPROPERTY(EditAnywhere)
	float DiscRadius;

	UPROPERTY(EditAnywhere)
	float MinRadius;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void DrawShape(TArray<int> stats);
};
