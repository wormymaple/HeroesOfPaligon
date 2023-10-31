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

void APlayerStats::ShowStats(FCharSave Character)
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	StatsText = AttachedActors[0]->GetComponentByClass<UTextRenderComponent>();
	FString statsString = "Health: " + StringHelper::IntToString(Character.Health) + "\nMana: " + StringHelper::IntToString(Character.Mana);
	
	StatsText->SetText(FText::FromString(statsString));

	TArray<int> coreStats = {
		Character.Vitality,
		Character.Might,
		Character.Soul,
		Character.Wit,
		Character.Haste
	};
	StatDisc->DrawShape(coreStats);
}

