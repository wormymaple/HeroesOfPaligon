// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugLogHelper.h"

DebugLogHelper::DebugLogHelper()
{
}

DebugLogHelper::~DebugLogHelper()
{
}

void DebugLogHelper::PrintString(FString Text, float Duration)
{
	GEngine->AddOnScreenDebugMessage(1, Duration, FColor::White, Text);
}
