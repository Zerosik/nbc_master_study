// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusComponent.h"
#include "Character/ShooterCharacter.h"

#pragma region ResourceStat
void FResourceStat::InitializeToMax()
{
	MaxValue = FMath::Max(0.0f, MaxValue);
	CurrentValue = MaxValue;
}

void FResourceStat::SetMaxValue(float NewMaxValue, bool bRefill)
{
	MaxValue = FMath::Max(0.0f, NewMaxValue);
	CurrentValue = bRefill ? MaxValue : FMath::Clamp(CurrentValue, 0.0f, MaxValue);
}

bool FResourceStat::Consume(float Amount, bool bAllowPartialConsume)
{
	if (Amount <= 0.0f)
	{
		return true;
	}

	if (bAllowPartialConsume)
	{
		if (CurrentValue <= 0.0f)
		{
			return false;
		}

		CurrentValue = FMath::Clamp(CurrentValue - Amount, 0.0f, MaxValue);
		return true;
	}

	if (CurrentValue < Amount)
	{
		return false;
	}

	CurrentValue = FMath::Clamp(CurrentValue - Amount, 0.0f, MaxValue);
	return true;
}

bool FResourceStat::Recover(float Amount)
{
	if (Amount <= 0.0f)
	{
		return false;
	}

	const float PreviousValue = CurrentValue;
	CurrentValue = FMath::Clamp(CurrentValue + Amount, 0.0f, MaxValue);
	return !FMath::IsNearlyEqual(CurrentValue, PreviousValue);
}

float FResourceStat::GetRatio() const
{
	return MaxValue > 0.0f ? CurrentValue / MaxValue : 0.0f;
}

bool FResourceStat::IsEmpty() const
{
	return CurrentValue <= 0.0f;
}

#pragma endregion ResourceStat

// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UStatusComponent::DamageTake);
	OnCharacterDeath.AddDynamic(this, &UStatusComponent::OnDeathEvent);
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UStatusComponent::DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer)
{
	float FinalDamage = FMath::Min(Damage, Health.CurrentValue);
	Health.Consume(FinalDamage, false);

	OnHealthChanged.Broadcast(Health.CurrentValue, Health.MaxValue);
	if (Health.CurrentValue <= 0.f) {
		OnCharacterDeath.Broadcast(Instigator);
	}
}

void UStatusComponent::OnDeathEvent(AController* Instigator)
{
// 	AShooterCharacter* OwnerCharacter = Cast<AShooterCharacter>(GetOwner());
// 	if (OwnerCharacter)
// 	{
// 		OwnerCharacter->PlayerOnDead();
// 	}
}