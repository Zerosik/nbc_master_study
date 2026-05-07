// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "WeaponTemplateBase.generated.h"

/**
 * 
 */
UCLASS()
class MASTER_0424_API AWeaponTemplateBase : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void Fire() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reload();

protected:
	UFUNCTION(BlueprintNativeEvent)
	bool CheckAmmo();

	///BlueprintImplementableEvent는 구현하면안됨, 블루프린트에서 구현하도록 위임하는것.
	//총을 어떻게 쏠지
	UFUNCTION(BlueprintImplementableEvent)
	void ProcessFiring();
	//이펙트 실행
	UFUNCTION(BlueprintImplementableEvent)
	void PlayEffects();
	//총알 업데이트
	UFUNCTION(BlueprintNativeEvent)
	void UpdateAmmo();
};
