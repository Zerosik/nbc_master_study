// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyItemData.generated.h"

/**
 * 
 */
UCLASS()
class MASTER_0424_API UMyItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId(ItemType, GetFName());
	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	FPrimaryAssetType ItemType;
	
	UPROPERTY(EditAnywhere, Category="Visual", meta=(AssetBundles = "Mesh"))
	TSoftObjectPtr<USkeletalMesh> ItemMesh;


	UPROPERTY(EditAnywhere, Category="Visual", meta = (AssetBundles = "Character"))
	TSoftObjectPtr<USkeletalMesh> ItemMesh2;

	UPROPERTY(EditAnywhere, Category="Stats")
	float AttackPower;
};

