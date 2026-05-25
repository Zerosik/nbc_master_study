// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "UI/PlayerHudWidget.h"
#include "Components/StatusComponent.h"
#include "Character/ShooterCharacter.h"

AShooterPlayerController::AShooterPlayerController()
    : InputMappingContext(nullptr)
    , MoveAction(nullptr)
    , JumpAction(nullptr)
    , LookAction(nullptr)
    , SprintAction(nullptr)
{
}

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void AShooterPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
}
void AShooterPlayerController::InitializeInput()
{
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (!LocalPlayer) return;

    auto* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
    if (Subsystem && InputMappingContext)
        Subsystem->AddMappingContext(InputMappingContext, 0);
}

void AShooterPlayerController::InitHUDWidget()
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
