// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexComponent.generated.h"

UENUM(BlueprintType)
enum class HexType
{
	None,
	Water,
	Sand,
	Ground,
	Rock
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VRTEST_API UHexComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHexComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void GetAdjacentHexes(float Radius, TArray<AActor*> Hexes);
	
	UPROPERTY(VisibleAnywhere)
	HexType Type = HexType::None;

	UFUNCTION(BlueprintCallable)
	TArray<UHexComponent*> ReturnAdjacentHexes();
	
	TArray<UHexComponent*> AdjacentHexes;

	void AddLoot(AActor* Loot);
	UPROPERTY()
	TArray<AActor*> LootActors;
};
