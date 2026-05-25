// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item_Cloth.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
void AItem_Cloth::OnFireDetected_Implementation(float Temperature, FVector HitLocation)
{
	if (FireEffect) {
		ParticleInstance = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			FireEffect,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.f)
		);
	}
}

void AItem_Cloth::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ParticleInstance->DestroyComponent();
	Super::EndPlay(EndPlayReason);
}
