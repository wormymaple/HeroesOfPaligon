// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveHandler.h"

#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

// Sets default values
ASaveHandler::ASaveHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASaveHandler::BeginPlay()
{
	Super::BeginPlay();

	FSaveState saveState;
	saveState.Race = 1;
	saveState.Color = 2;

	FString json = FString("");
	FJsonObjectConverter::UStructToJsonObjectString(saveState, json, 0, 0);

	GEngine->AddOnScreenDebugMessage(0, 10, FColor::White, json);
}

// Called every frame
void ASaveHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

