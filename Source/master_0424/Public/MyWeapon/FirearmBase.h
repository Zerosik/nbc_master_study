// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirearmBase.generated.h"

class AShooterCharacter;
class ABulletActor;

UCLASS()
class MASTER_0424_API AFirearmBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AFirearmBase();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UArrowComponent> FirePoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
    TSubclassOf<ABulletActor> ProjectileClass;


protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:

	virtual void Fire();
	virtual void Reload();

protected:

	bool CanFire();//발사가능여부

	void FireBullet();//발사

	void ConsumeAmmo();//총알 소모

	bool CheckAmmo();

	UFUNCTION(BlueprintNativeEvent)
	void PlayFireEffect();//이펙트

	UFUNCTION(BlueprintNativeEvent)
	void PlayFireSound();//사운드

	void RecoilEffect();//총기반동

	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Ammo")
	int32 CurrentAmmo = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Ammo")
	int32 MaxAmmo = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Ammo")
	float Range = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Ammo")
	float Damage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Ammo")
	int32 Pellets = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Spread")
	float MinSpread = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Spread")
	float MaxSpread = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Spread")
	float SpreadPerShot = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Spread")
	float SpreadRecovery = 5.f;
	float CurrentSpread;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Recoil")
	float VerticalRecoil=2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Recoil")
	float HorizontalRecoil=1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm|Recoil")
	float RecoverRecoilSpeed = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm")
	float ReloadTime = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FireArm")
	float FireRate = 0.1f;

	FTimerHandle TimerFireDelayHandle;//사격 딜레이
	FTimerHandle TimerReloadDelayHandle;//장전 딜레이
private:

	UPROPERTY()
	AShooterCharacter* OwnerCharacter;

	UFUNCTION()
	void HandleFireDelay();

	bool bCanShot = true;
	bool bReloading = false;
	float RecoverRecoil = 0.f;

};
