#pragma once

#include "CoreMinimal.h"
#include "PooledObjectData.generated.h"

USTRUCT(BlueprintType)
struct FPooledObjectData
{
	GENERATED_BODY()

	FPooledObjectData() {
		ActorTemplate = nullptr;
		PoolSize = 32;
		bCanGrow = false;
		ActorName = "default";
	}

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorTemplate;

	UPROPERTY(EditAnywhere)
	int32 PoolSize;
	
	UPROPERTY(EditAnywhere)
	bool bCanGrow;
	
	UPROPERTY(EditAnywhere)
	FString ActorName;


};