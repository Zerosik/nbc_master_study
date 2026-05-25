// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Interface/TestMyInterface.h"
#include "Item_Cloth.generated.h"

/**
 * 
 */
UCLASS()
class MASTER_0424_API AItem_Cloth : public AItemBase, public ITestMyInterface
{
	GENERATED_BODY()
public:
	virtual void OnFireDetected_Implementation(float Temperature, FVector HitLocation) override;

protected:
	UPROPERTY(EditAnywhere, Category="Effects")
	TObjectPtr<class UParticleSystem> FireEffect;
private:
	UPROPERTY()
	UParticleSystemComponent* ParticleInstance;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
