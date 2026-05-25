// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestActor.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	LoadWithSoftPtr();
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor::LoadWithSoftPtr()
{
	if (MySoftMesh.IsNull())
		return;

	UStaticMesh* FinalMesh = nullptr;
	if (MySoftMesh.IsValid()) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, FString::Printf(TEXT("이미 로드되어있음, 즉시참조함")));
		OnLoadCompleted();
	}
	else {
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(MySoftMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ATestActor::OnLoadCompleted));


		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, FString::Printf(TEXT("메모리에 없어 로드를 실행함.")));
	}
	if (FinalMesh) {
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, FString::Printf(TEXT("FinalMesh 있음. : %s"), *FinalMesh->GetName()));
	}
}

void ATestActor::OnLoadCompleted()
{
	UStaticMesh* FinalMesh = MySoftMesh.Get();
}

