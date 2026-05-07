// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "Components/ArrowComponent.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(Root);

	AmmoPerFire = 1;
	CurrentAmmo = 0;
	MaxAmmo = 12;
	RateOfFire = 1.f;
	CanFire = true;
	Range = 1000.f;
	DamagePerHit = 100.f;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Fire()
{
	CanFire = false;
	GetWorldTimerManager().SetTimer(
		TimerFireDelay,
		this,
		&AWeaponBase::HandleFireDelay,
		1.f / RateOfFire,
		false);
}

void AWeaponBase::HandleFireDelay()
{
	GetWorldTimerManager().ClearTimer(TimerFireDelay);
	CanFire = true;
}

