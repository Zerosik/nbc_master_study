// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/MyDataAssetTestActor.h"
#include "DataAsset/MyItemData.h"
#include "Engine/AssetManager.h"

// Sets default values
AMyDataAssetTestActor::AMyDataAssetTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMyDataAssetTestActor::BeginPlay()
{
	Super::BeginPlay();
	StartLoading();
}


void AMyDataAssetTestActor::StartLoading()
{
	if (!ItemToLoad.IsValid())return;

	UAssetManager& AssetManeger = UAssetManager::Get();
	
	TArray<FName> bundles;
	bundles.Add(FName("Mesh"));

	LoadingHandle = AssetManeger.LoadPrimaryAsset(ItemToLoad, bundles,
		FStreamableDelegate::CreateUObject(this, &AMyDataAssetTestActor::OnLoadFinishedAsync, ItemToLoad)
	);
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, FString::Printf(TEXT("아이템 로드 시작")));

}

void AMyDataAssetTestActor::OnLoadFinishedAsync(FPrimaryAssetId LoadedId)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, FString::Printf(TEXT("아이템 로드 성공")));

	LoadedItem = Cast<UMyItemData>(UAssetManager::Get().GetPrimaryAssetObject(LoadedId));

	if (LoadedItem)
	{
		if (LoadedItem->ItemMesh) {
			FString Msg = FString::Printf(TEXT("데이터에셋 로드 이름 : %s"), *LoadedItem->ItemMesh.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, Msg);
		}
		if (LoadedItem->ItemMesh2) {
			FString Msg = FString::Printf(TEXT("데이터에셋2 로드 이름 : %s"), *LoadedItem->ItemMesh2.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Orange, Msg);
		}
	}

}

