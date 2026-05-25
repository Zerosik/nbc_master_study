// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHudWidget.generated.h"

class UStatusComponent;
class UProgressBar;
UCLASS()
class MASTER_0424_API UPlayerHudWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION()
    void InitWidget(UStatusComponent* InHealthComp);

    UFUNCTION()
    void OnHealthChanged(float CurrentHealth, float MaxHealth);

protected:
    virtual void NativeConstruct() override;

private:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> HP_Bar;
};
