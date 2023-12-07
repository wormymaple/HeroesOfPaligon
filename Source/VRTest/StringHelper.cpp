// Fill out your copyright notice in the Description page of Project Settings.


#include "StringHelper.h"

StringHelper::StringHelper()
{
}

StringHelper::~StringHelper()
{
}

FString StringHelper::IntToString(int n)
{
	return FString::Printf(TEXT("%i"), n);
}

FString StringHelper::VectorToString(FVector v)
{
	return FString::Printf(TEXT("%f"), v.X) + ", " + FString::Printf(TEXT("%f"), v.Y) + ", " + FString::Printf(TEXT("%f"), v.Z);
}
