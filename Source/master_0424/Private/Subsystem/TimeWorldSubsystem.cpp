// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/TimeWorldSubsystem.h"

int32 UTimeWorldSubsystem::GetCurrentHour()
{
	UWorld* World = GetWorld();

	if (!World)
		return 0;

	float TotalSeconds = World->GetTimeSeconds();

	int32 Hours = FMath::FloorToInt32(TotalSeconds * 60.f);

	return Hours%24;

}
