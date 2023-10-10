// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveHandler.h"
#include "Components/TextRenderComponent.h"

#include "GameFramework/Actor.h"
#include "PlayerStats.generated.h"

UCLASS()
class VRTEST_API APlayerStats : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerStats();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* StatsText;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    void ShowStats(FPlayerPackage playerPackage);

};
