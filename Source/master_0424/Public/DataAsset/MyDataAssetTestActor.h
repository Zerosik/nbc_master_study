// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/AssetManager.h"
#include "MyDataAssetTestActor.generated.h"

class UMyItemData;

UCLASS()
class MASTER_0424_API AMyDataAssetTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDataAssetTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, Category="ItemTest")
	FPrimaryAssetId ItemToLoad;

	UPROPERTY(VisibleAnywhere, Category="ItemTest")
	UMyItemData* LoadedItem;

	void StartLoading();

	void OnLoadFinishedAsync(FPrimaryAssetId LoadedId);

	TSharedPtr<FStreamableHandle> LoadingHandle;


};
