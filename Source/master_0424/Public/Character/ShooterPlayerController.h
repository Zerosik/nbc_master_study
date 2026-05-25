// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UPlayerHudWidget;

UCLASS()
class MASTER_0424_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AShooterPlayerController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void InitializeInput();
	void InitHUDWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* BasicAttackAction;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHudWidget> HUDWidgetClass;
private:
	UPROPERTY()
	TObjectPtr<UPlayerHudWidget> HUDWidgetInstance;


};
