// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveHandler.generated.h"

USTRUCT()
struct FSaveState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int Race;
	UPROPERTY()
	int Color;
};

UCLASS()
class VRTEST_API ASaveHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
