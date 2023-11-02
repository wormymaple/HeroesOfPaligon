// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatDisc.h"
#include "GameFramework/Actor.h"

#include "SaveHandler.generated.h"


class APlayerStats;

USTRUCT()
struct FCharSave
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Name;
	
	UPROPERTY()
	int Race;
	UPROPERTY()
	int Color;

	UPROPERTY()
	int Health;
	UPROPERTY()
	int Mana;
	
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

	UPROPERTY()
	int Personality;

	FCharSave() { Name = TEXT("Steve the Stoic"); } 
	
	FCharSave(FString CharName, TArray<int> VanityStats, TArray<int> ResourceStats, TArray<int> CoreStats, int CharPersonality)
	{
		Name = CharName;

		Race = VanityStats[0];
		Color = VanityStats[1];

		Health = ResourceStats[0];
		Mana = ResourceStats[1];

		Wit = ResourceStats[0];
		Might = ResourceStats[1];
		Soul = ResourceStats[2];
		Haste = ResourceStats[3];
		Vitality = ResourceStats[4];

		Personality = CharPersonality;
	}
};

USTRUCT()
struct FGameSave
{
	GENERATED_BODY()
	
	UPROPERTY()
	int SaveID = 1;

	UPROPERTY()
	FString LastBiome = TEXT("None");

	FGameSave() { }
	FGameSave(int ID, FString Biome = TEXT("None"))
	{
		SaveID = ID;
		LastBiome = Biome;
	}
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

	void SaveChars(TArray<FCharSave> Characters);
	TArray<FCharSave> ReadChars();
	
	void SaveGame(FGameSave GameSave);
	FGameSave ReadGame(int SaveID);

	UPROPERTY(EditAnywhere)
	bool IsInCampfire;
};
