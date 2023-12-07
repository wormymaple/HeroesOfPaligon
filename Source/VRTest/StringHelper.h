// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class VRTEST_API StringHelper
{
public:
	StringHelper();
	~StringHelper();

	static FString IntToString(int n);
	static FString VectorToString(FVector v);
};
