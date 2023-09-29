// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveHandler.h"

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

	FPlayerPackage newPlayer = FPlayerPackage();
	newPlayer.CharSaves.Add(FSaveState());
	newPlayer.PlayerInfo.UniqueID = 0;
	
	SaveGame(TArray<FPlayerPackage> {newPlayer});
}

// Called every frame
void ASaveHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaveHandler::SaveGame(TArray<FPlayerPackage> Players, int SaveID)
{
	if (SaveID == 0) SaveID = FMath::RandRange(0, 65536);

	FString CurrentSaveDir = SaveDir + "/SavedGame_" + FString::Printf(TEXT("%i"), SaveID);

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

	CreateDir(CurrentSaveDir, platformFile);

	FWorldState newWorldState;
	newWorldState.SaveID = SaveID;

	FString worldJson = "";
	FJsonObjectConverter::UStructToJsonObjectString(newWorldState, worldJson, 0, 0);

	FFileHelper::SaveStringToFile(worldJson, *(CurrentSaveDir + "/WorldData.json"));

	for (FPlayerPackage player : Players)
	{
		int UniqueID = player.PlayerInfo.UniqueID;

		FString playerSaveDir = CurrentSaveDir + "/Player_" + FString::Printf(TEXT("%i"), UniqueID);
		CreateDir(playerSaveDir, platformFile);

		FString playerInfoJson = "";
		FJsonObjectConverter::UStructToJsonObjectString(player.PlayerInfo, playerInfoJson, 0, 0);
		FFileHelper::SaveStringToFile(playerInfoJson, *(playerSaveDir + "/PlayerInfo.json"));

		int saveCount = 0;
		for (FSaveState saveState : player.CharSaves)
		{
			FString fileName = "/SaveState_" + FString::Printf(TEXT("%i"), saveCount) + ".json";
			FString saveStateJson = "";
			FJsonObjectConverter::UStructToJsonObjectString(saveState, saveStateJson, 0, 0);
			FFileHelper::SaveStringToFile(saveStateJson, *(playerSaveDir + fileName));

			saveCount += 1;
		}
	}
}

void ASaveHandler::CreateDir(FString Dir, IPlatformFile& platformFile)
{
	if (!platformFile.DirectoryExists(*Dir))
	{
		platformFile.CreateDirectory(*Dir);
	}
}



