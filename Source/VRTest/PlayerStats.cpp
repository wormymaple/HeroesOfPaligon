// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStats.h"

#include "StringHelper.h"
#include "Components/TextRenderComponent.h"

// Sets default values
APlayerStats::APlayerStats()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerStats::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerStats::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerStats::ShowStats(FPlayerPackage playerPackage)
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	StatsText = AttachedActors[0]->GetComponentByClass<UTextRenderComponent>();

	FSaveState currentChar = playerPackage.CharSaves[playerPackage.PlayerInfo.UsedCharacter];
	FString statsString = "Health: " + StringHelper::IntToString(currentChar.Health) + "\nMana: " + StringHelper::IntToString(currentChar.Mana);
	
	StatsText->SetText(FText::FromString(statsString));

	TArray<int> coreStats = {
		currentChar.Vitality,
		currentChar.Might,
		currentChar.Soul,
		currentChar.Wit,
		currentChar.Haste
	};
	StatDisc->DrawShape(coreStats);
}

