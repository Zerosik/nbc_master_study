// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "FireDamageType.generated.h"

/**
 * 
 */
UCLASS()
class MASTER_0424_API UFireDamageType : public UDamageType
{
	GENERATED_BODY()
public:
	UFireDamageType();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BurnDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmorPenetration;

};
