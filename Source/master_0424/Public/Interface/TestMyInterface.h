// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestMyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTestMyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MASTER_0424_API ITestMyInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interface")
	void OnFireDetected(float Temperature, FVector HitLocation);
};
