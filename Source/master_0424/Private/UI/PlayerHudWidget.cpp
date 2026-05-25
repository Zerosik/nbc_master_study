// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHudWidget.h"
#include "Components/ProgressBar.h"
#include "Components/StatusComponent.h"

void UPlayerHudWidget::InitWidget(UStatusComponent* StatusComponent)
{
    if (!StatusComponent)
        return;

    StatusComponent->OnHealthChanged.AddDynamic(
        this, &UPlayerHudWidget::OnHealthChanged);
}

void UPlayerHudWidget::OnHealthChanged(float CurrentHealth, float MaxHealth)
{
    if (HP_Bar && MaxHealth > 0.f)
    {
        HP_Bar->SetPercent(CurrentHealth / MaxHealth);
    }
}

void UPlayerHudWidget::NativeConstruct()
{
    Super::NativeConstruct();
}
