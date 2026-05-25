// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Item/ItemBase.h"

UMyActorComponent::UMyActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UMyActorComponent::DamageTake);
	OnDeath.AddDynamic(this, &UMyActorComponent::OnDeathEvent);
}


// Called every frame
void UMyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Red, FString::Printf(TEXT("Health : %f"), CurrentHealth));
}


void UMyActorComponent::DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer)
{
	float FinalDamage = FMath::Min(Damage, CurrentHealth);
	CurrentHealth -= FinalDamage;

	OnHealthDamaged.Broadcast(CurrentHealth, MaxHealth, FinalDamage);
	if (CurrentHealth <= 0.f) {
		OnDeath.Broadcast(Instigator);
	}
}

void UMyActorComponent::OnDeathEvent(AController* Instigator)
{
	TArray<AActor*> Items;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemBase::StaticClass(), Items);
	for (AActor* item : Items) {
		item->Destroy();
	}
}
