// Fill out your copyright notice in the Description page of Project Settings.


#include "HexComponent.h"

// Sets default values for this component's properties
UHexComponent::UHexComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHexComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHexComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHexComponent::GetAdjacentHexes(float Radius, TArray<AActor*> Hexes)
{
	FVector2D hexPos = FVector2D(GetOwner()->GetActorLocation());
	AdjacentHexes.Empty();
	for (AActor* hex : Hexes)
	{
		float dist = FVector2D::Distance(hexPos, FVector2D(hex->GetActorLocation())); 
		if (dist <= Radius && dist > 2)
		{
			AdjacentHexes.Add(hex->GetComponentByClass<UHexComponent>());
		}
	}
}


