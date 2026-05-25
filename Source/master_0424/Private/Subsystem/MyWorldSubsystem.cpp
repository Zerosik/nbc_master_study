// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MyWorldSubsystem.h"
#include "Subsystem/TimeWorldSubsystem.h"

void UMyWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Warning, TEXT("WorldSubsystem Initialize"));

	UTimeWorldSubsystem* TimeSystem = Collection.InitializeDependency<UTimeWorldSubsystem>();
	
	if (TimeSystem) {
		int32 CurrentHours = TimeSystem->GetCurrentHour();
		UE_LOG(LogTemp, Warning, TEXT("Current Hour : %d"), CurrentHours);
	}

}

void UMyWorldSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("WorldSubsystem Deinitialize"));


	Super::Deinitialize();
}

bool UMyWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	UWorld* World = Cast<UWorld>(Outer);
	bool bIsStartMap = World->GetMapName().Contains(TEXT("StarterMap"));
	UE_LOG(LogTemp, Warning, TEXT("%d"), bIsStartMap);
	return bIsStartMap;
}

