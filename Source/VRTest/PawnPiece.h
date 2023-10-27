// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameBoardManager.h"
#include "HexComponent.h"
#include "PawnPiece.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class VRTEST_API UPawnPiece : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPawnPiece();

protected:
	UPROPERTY()
	UHexComponent* CurrentHex;
	virtual void BeginPlay() override;

public:
	
	UPROPERTY(BlueprintReadWrite)
	AGameBoardManager* BoardManager;

	UHexComponent* GetCurrentHex();
	void SetCurrentHex(UHexComponent* InHex);

	FSaveState CurrentCharacter;
};
