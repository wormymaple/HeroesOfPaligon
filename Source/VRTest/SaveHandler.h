// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SaveHandler.generated.h"


class APlayerStats;

USTRUCT()
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int UniqueID;

	UPROPERTY()
	int UsedCharacter;
};

USTRUCT()
struct FSaveState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int Race;
	UPROPERTY()
	int Color;

	UPROPERTY()
	int Mana;
	UPROPERTY()
	int Health;
	
	UPROPERTY()
	int Wit;
	UPROPERTY()
	int Might;
	UPROPERTY()
	int Soul;
	UPROPERTY()
	int Haste;
	UPROPERTY()
	int Vitality;
};

USTRUCT()
struct FPlayerPackage
{
	GENERATED_BODY()

	UPROPERTY()
	FPlayerInfo PlayerInfo;

	UPROPERTY()
	TArray<FSaveState> CharSaves;
};

USTRUCT()
struct FWorldState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int SaveID;
	
	UPROPERTY()
	int LastBiome;
};

USTRUCT()
struct FGameSave
{
	GENERATED_BODY()
	UPROPERTY()
	FWorldState WorldState;

	UPROPERTY()
	TArray<FPlayerPackage> PlayerPackages;
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
	FString SaveDir;

	void CreateDir(FString Dir, IPlatformFile& platformFile);

	UPROPERTY(EditAnywhere)
	APlayerStats* PlayerStats;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SaveGame(TArray<FPlayerPackage> Players, int SaveID = 0);
	FGameSave ReadGame(int SaveID);

};
