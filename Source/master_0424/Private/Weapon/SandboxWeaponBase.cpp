// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/SandboxWeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void ASandboxWeaponBase::Fire()
{
	SandboxFire();
}


void ASandboxWeaponBase::Reload()
{
	CurrentAmmo = MaxAmmo;
	HandleFireDelay();

}

bool ASandboxWeaponBase::CheckAmmo()
{
	return CurrentAmmo >= AmmoPerFire;
}

void ASandboxWeaponBase::LinetraceOneShot(FVector Direction)
{
	FHitResult Hit(ForceInit);

	FVector Start = FirePoint->GetComponentLocation();
	FVector End = Start + (Direction * Range);

	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		{ this, GetOwner() },
		EDrawDebugTrace::ForDuration,
		Hit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1.f
	);
}

void ASandboxWeaponBase::PlaySound(USoundBase* Sound)
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
}

void ASandboxWeaponBase::UpdateAmmo()
{
	CurrentAmmo -= AmmoPerFire;
}
