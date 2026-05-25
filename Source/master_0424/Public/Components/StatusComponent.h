// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeath, AController*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);


USTRUCT(BlueprintType)
struct MASTER_0424_API FResourceStat
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Resource")
	float MaxValue = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Resource")
	float CurrentValue = 100.f;

	void InitializeToMax();
	void SetMaxValue(float NewMaxValue, bool bRefill);
	bool Consume(float Amount, bool bAllowPartialConsume);
	bool Recover(float Amount);
	float GetRatio() const;
	bool IsEmpty() const;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTER_0424_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
	FResourceStat Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	bool bDead = false;

	
	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeath OnCharacterDeath;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;


protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer);
	UFUNCTION()
	void OnDeathEvent(AController* Instigator);
		
};
