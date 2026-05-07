// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TraceTest.generated.h"

UCLASS()
class MASTER_0424_API ATraceTest : public AActor
{
	GENERATED_BODY()
	
public:	
	ATraceTest();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	void StartSingleTrace();

	void StartAsyncTrace();

	void OnAsyncTraceCompleted(const FTraceHandle& Handle, FTraceDatum& data);

	void ShotgunTrace();
private:
	FTimerHandle TimerHandle;
};
