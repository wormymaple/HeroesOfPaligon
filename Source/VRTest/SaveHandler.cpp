// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveHandler.h"

#include "PlayerStats.h"
#include "StringHelper.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SaveDir = nullptr;
}

// Called when the game starts or when spawned
void ASaveHandler::BeginPlay()
{
	Super::BeginPlay();

	FString dir = FPaths::ProjectSavedDir() + "/SavedGames";
	SaveDir = dir;

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (!platformFile.DirectoryExists(*SaveDir))
	{
		platformFile.CreateDirectory(*SaveDir);
	}

	if (IsInCampfire)
	{
		FGameSave readGame = ReadGame(1);
	}
}

// Called every frame
void ASaveHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaveHandler::SaveChars(TArray<FCharSave> Characters)
{
	FString charDir = SaveDir + "/CharacterSaves";

	int i = 0;
	for (FCharSave character : Characters)
	{
		FString json = "";
		FJsonObjectConverter::UStructToJsonObjectString(character, json, 0, 0);

		FFileHelper::SaveStringToFile(json, *(SaveDir + "/CharSave_" + StringHelper::IntToString(i) + ".json"));

		i += 1;
	}
}

TArray<FCharSave> ASaveHandler::ReadChars()
{
	TArray<FCharSave> readChars;

	for (int i = 0; i < 3; i += 1)
	{
		FString path = SaveDir + "/CharSave_" + StringHelper::IntToString(i) + ".json";
		if (!FPaths::FileExists(*path)) continue;

		FString json = "";
		FFileHelper::LoadFileToString(json, *path);

		FCharSave readChar;
		FJsonObjectConverter::JsonObjectStringToUStruct(json, &readChar, 0, 0);
		readChars.Add(readChar);
	}

	return readChars;
}

void ASaveHandler::SaveGame(FGameSave GameSave)
{
	FString gameSaveDir = SaveDir + "/GameSaves/Game_" + StringHelper::IntToString(GameSave.SaveID) + ".json";
	
	FString json = "";
	FJsonObjectConverter::UStructToJsonObjectString(GameSave, json, 0, 0);

	FFileHelper::SaveStringToFile(json, *gameSaveDir);
}

FGameSave ASaveHandler::ReadGame(int SaveID)
{
	FString gameSaveDir = SaveDir + "/GameSaves/Game_" + StringHelper::IntToString(SaveID) + ".json";

	if (!FPaths::FileExists(gameSaveDir)) return FGameSave(SaveID, "None");
	
	FString json = "";
	FFileHelper::LoadFileToString(json, *gameSaveDir);
	FGameSave readSave;
	FJsonObjectConverter::JsonObjectStringToUStruct(json, &readSave, 0, 0);

	return readSave;
}

void ASaveHandler::CreateDir(FString Dir, IPlatformFile& platformFile)
{
	if (!platformFile.DirectoryExists(*Dir))
	{
		platformFile.CreateDirectory(*Dir);
	}
}



