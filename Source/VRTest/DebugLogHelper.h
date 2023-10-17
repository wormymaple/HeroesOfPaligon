// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class VRTEST_API DebugLogHelper
{
public:
	DebugLogHelper();
	~DebugLogHelper();

	static void PrintString(FString Text, float Duration = 1);
};
