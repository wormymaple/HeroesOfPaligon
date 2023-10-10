// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveHandler.h"

#include "PlayerStats.h"
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

	FSaveState newChar;
	newChar.Mana = 15;
	newChar.Health = 15;

	newChar.Wit = 5;
	newChar.Might = 7;
	newChar.Soul = 4;
	newChar.Haste = 10;
	newChar.Vitality = 11;
	
	FPlayerPackage newPlayer;
	newPlayer.CharSaves = TArray<FSaveState> {newChar};

	newPlayer.PlayerInfo.UniqueID = 0;
	newPlayer.PlayerInfo.UsedCharacter = 0;
	
	SaveGame(TArray<FPlayerPackage> {newPlayer}, 22119);
	
	FGameSave readGame = ReadGame(22119);

	PlayerStats->ShowStats(readGame.PlayerPackages[0]);
}

// Called every frame
void ASaveHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaveHandler::SaveGame(TArray<FPlayerPackage> Players, int SaveID)
{
	if (SaveID == 0) SaveID = FMath::RandRange(0, 65536);

	FWorldState newWorldState;
	newWorldState.SaveID = SaveID;

	FGameSave newGameSave;
	newGameSave.WorldState = newWorldState;
	newGameSave.PlayerPackages = Players;
	
	FString json = "";
	FJsonObjectConverter::UStructToJsonObjectString(newGameSave, json, 0, 0);

	FFileHelper::SaveStringToFile(json, *(SaveDir + "/GameSave" + FString::Printf(TEXT("%i"), SaveID) + ".json"));
}

FGameSave ASaveHandler::ReadGame(int SaveID)
{
	FString json = "";
	FFileHelper::LoadFileToString(json, *(SaveDir + "/GameSave" + FString::Printf(TEXT("%i"), SaveID) + ".json"));
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



