// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MasterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/PlayerHudWidget.h"
#include "Components/StatusComponent.h"
#include "Character/ShooterCharacter.h"

AMasterPlayerController::AMasterPlayerController()
	:InputMappingContext(nullptr), 
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr),
	SprintAction(nullptr),
	AimingAction(nullptr),
	FireAction(nullptr)
{

}
void AMasterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* localPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem =
			localPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				SubSystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

	InitHUDWidget();
}

void AMasterPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMasterPlayerController::InitializeInput()
{

}
void AMasterPlayerController::InitHUDWidget()
{
	if (HUDWidgetInstance || !HUDWidgetClass) return;

	HUDWidgetInstance = CreateWidget<UPlayerHudWidget>(this, HUDWidgetClass);
	if (!HUDWidgetInstance) return;

	HUDWidgetInstance->AddToViewport();

	AShooterCharacter* PlayerCharacter = Cast<AShooterCharacter>(GetPawn());
	if (!PlayerCharacter) return;

	UStatusComponent* StatusComponent = PlayerCharacter->GetStatusComponent();
	if (!StatusComponent) return;
	HUDWidgetInstance->InitWidget(StatusComponent);
}
