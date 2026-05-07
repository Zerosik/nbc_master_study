// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class MASTER_0424_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, Category = "Weapon")
    TSubclassOf<class AActor> WeaponClass;


    UPROPERTY()
    class AActor* AttachedWeapon;

	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsAiming() const;
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsSprinting() const;
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsWalking() const;

	
	UFUNCTION()
	FVector GetCameraTargetLocation();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category="Movement")
	float WalkSpeed=300.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float RuningSpeed = 600.f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float SprintSPeedMultiplier = 1.5f;
	UPROPERTY(EditAnywhere, Category="Movement")
	float SprintSpeed=900.f;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);
	UFUNCTION()
	void StartAiming(const FInputActionValue& value);
	UFUNCTION()
	void StopAiming(const FInputActionValue& value);
	UFUNCTION()
	void Fire(const FInputActionValue& value);

private:
	bool bIsAiming = false;
	bool bIsSprinting = false;
	bool bIsWalking = false;

};
