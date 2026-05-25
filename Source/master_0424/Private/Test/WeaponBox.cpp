// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/WeaponBox.h"
#include "Test/MyWeaponStruct.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
// Sets default values
AWeaponBox::AWeaponBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponBox::BeginPlay()
{
	Super::BeginPlay();
	OpneBox();
}

// Called every frame
void AWeaponBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBox::OpneBox()
{
	if (!WeaponTable)return;

	TArray<FWeaponData*> AllWeapons;

	WeaponTable->GetAllRows<FWeaponData>(TEXT("Weapon Load Fail"), AllWeapons);

	if (AllWeapons.Num() == 0)return;
	FWeaponData* SelecetedWeapon = AllWeapons[FMath::RandRange(0, AllWeapons.Num() - 1)];

	if (SelecetedWeapon) {
		SoftSpawnWeapon = SelecetedWeapon->WeaponClass;
		FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
		Streamable.RequestAsyncLoad(SoftSpawnWeapon.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &AWeaponBox::SpawnWeaponAsync));

// 		UClass* LoadedClass = SelecetedWeapon->WeaponClass.LoadSynchronous();
// 		if (LoadedClass) {
// 			GetWorld()->SpawnActor<AActor>(LoadedClass, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator);
// 			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("생성완료 : %s"), *SelecetedWeapon->WeaponName));
// 		}
	}
}

void AWeaponBox::SpawnWeaponAsync()
{
	UClass* LoadedClass = SoftSpawnWeapon.Get();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (LoadedClass) {
		GetWorld()->SpawnActor<AActor>(LoadedClass, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator, SpawnParams);
		
	}
}

