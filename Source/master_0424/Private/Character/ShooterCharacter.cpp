// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/MasterPlayerController.h"
#include "Weapon/SandboxWeaponBase.h"
#include "MyWeapon/FirearmBase.h"


// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//스프링암을 루트컴포넌트에 부착하고
	SpringArmComp->SetupAttachment(RootComponent);
	//캐릭터와 거리를 300
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//카메라를 스프링암에 부착, 다만 소캣에 부착하는데 이는 스프링암에 끝부분에 부착하는걸 의미함.
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	RuningSpeed = 600.f;
	SprintSPeedMultiplier = 1.6f;
	SprintSpeed = RuningSpeed * SprintSPeedMultiplier;
	GetCharacterMovement()->MaxWalkSpeed = RuningSpeed;
}
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (WeaponClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// 무기 스폰
			AActor* Weapon = World->SpawnActor<AActor>(WeaponClass);
			Weapon->SetActorEnableCollision(false);
			Weapon->SetActorRotation(FRotator{ 0,180,0 });
			if (Weapon)
			{
				// 캐릭터 메시의 소켓에 부착
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
				Weapon->AttachToComponent(GetMesh(), AttachmentRules, FName("ik_hand_gunSocket"));
				AttachedWeapon = Weapon;
			}
		}
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AShooterCharacter::IsAiming() const
{
	return bIsAiming;
}

bool AShooterCharacter::IsSprinting() const
{
	return bIsSprinting;
}

bool AShooterCharacter::IsWalking() const
{
	return bIsWalking;
}



// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (AMasterPlayerController* PlayerController = Cast<AMasterPlayerController>(GetController())) {
			//이동
			if (PlayerController->MoveAction) {
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::Move
				);
			}
			//점프 시작
			if (PlayerController->JumpAction) {
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::StartJump
				);
			}
			//점프 종료
			if (PlayerController->JumpAction) {
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AShooterCharacter::StopJump
				);
			}
			//마우스 회전
			if (PlayerController->LookAction) {
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::Look
				);
			}
			//달리기 시작
			if (PlayerController->SprintAction) {
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::StartSprint
				);
			}
			//달리기 종료
			if (PlayerController->SprintAction) {
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&AShooterCharacter::StopSprint
				);
			}
			if (PlayerController->AimingAction) {
				EnhancedInput->BindAction(
					PlayerController->AimingAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::StartAiming
				);
			}
			//점프 종료
			if (PlayerController->AimingAction) {
				EnhancedInput->BindAction(
					PlayerController->AimingAction,
					ETriggerEvent::Completed,
					this,
					&AShooterCharacter::StopAiming
				);
			}
			if (PlayerController->FireAction) {
				EnhancedInput->BindAction(
					PlayerController->FireAction,
					ETriggerEvent::Triggered,
					this,
					&AShooterCharacter::Fire
				);
			}
		}
	}
}

void AShooterCharacter::Move(const FInputActionValue& value) {
	if (!Controller)return;
	const FVector2D moveInput = value.Get<FVector2D>();
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("X:%f, Y:%f"), moveInput.X, moveInput.Y));
	if (!FMath::IsNearlyZero(moveInput.X)) {
		AddMovementInput(GetActorForwardVector(), moveInput.X);
	}
	if (!FMath::IsNearlyZero(moveInput.Y)) {
		AddMovementInput(GetActorRightVector(), moveInput.Y);
	}
}
void AShooterCharacter::StartJump(const FInputActionValue& value) {
	if (value.Get<bool>()) {
		Jump();
	}
}
void AShooterCharacter::StopJump(const FInputActionValue& value) {
	if (!value.Get<bool>()) {
		StopJumping();
	}

}
void AShooterCharacter::Look(const FInputActionValue& value) {

	FVector2D LookAxisVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
void AShooterCharacter::StartSprint(const FInputActionValue& value) {
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}

}
void AShooterCharacter::StopSprint(const FInputActionValue& value) {
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeed = RuningSpeed;
	}
}
void AShooterCharacter::StartAiming(const FInputActionValue& value) {
	bIsAiming = true;
}

void AShooterCharacter::StopAiming(const FInputActionValue& value) {
	bIsAiming = false;
}

void AShooterCharacter::Fire(const FInputActionValue& value)
{
	ASandboxWeaponBase* Weapon = Cast<ASandboxWeaponBase>(AttachedWeapon);
	if (Weapon) {
		Weapon->SandboxFire();
	}	
	AFirearmBase* Weapon2 = Cast<AFirearmBase>(AttachedWeapon);
	if (Weapon2) {
		Weapon2->Fire();
	}
}

FVector AShooterCharacter::GetCameraTargetLocation()
{
	UWorld* const World = GetWorld();

	if (World != nullptr)
	{
		AController* MyController = GetController();
		if (!MyController)
		{
			return FVector();
		}
		FVector ViewLocation;
		FRotator ViewRotation;

		MyController->GetPlayerViewPoint(ViewLocation, ViewRotation);
		float AimTraceDistance = 10000.f;//
		const FVector TraceStart = ViewLocation;//카메라지점
		const FVector TraceEnd = TraceStart + ViewRotation.Vector() * AimTraceDistance;//카메라가 바라보는 지점 10000거리까지

		FHitResult HitResult;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		const bool bHit = World->LineTraceSingleByChannel(
			HitResult,
			TraceStart,
			TraceEnd,
			ECC_Visibility,
			QueryParams
		);

		FVector TargetLocation;

		if (bHit)
		{
			TargetLocation = HitResult.ImpactPoint;
		}
		else
		{
			TargetLocation = TraceEnd;
		}
		return TargetLocation;
	}
	else {
		return FVector();
	}

}
