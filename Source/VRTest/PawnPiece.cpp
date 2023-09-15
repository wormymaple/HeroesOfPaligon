// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPiece.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPawnPiece::UPawnPiece()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPawnPiece::BeginPlay()
{
	Super::BeginPlay();
}


UHexComponent* UPawnPiece::GetCurrentHex()
{
	return CurrentHex;
}

void UPawnPiece::SetCurrentHex(UHexComponent* InHex)
{
	CurrentHex = InHex;
}

