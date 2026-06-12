// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon/FirearmBase.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "Character/ShooterCharacter.h"
#include "Subsystem/ObjectPoolSubsystem.h"
#include "MyWeapon/BulletActor.h"
// Sets default values
AFirearmBase::AFirearmBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	FirePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(Root);
	
}
void AFirearmBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AShooterCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	CurrentSpread = MinSpread;
	
	
	PoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
}

void AFirearmBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//반동 회복
	if (!FMath::IsNearlyZero(RecoverRecoil))
	{
		float RecoveryAmount = FMath::FInterpTo(RecoverRecoil, 0.0f, DeltaTime, RecoverRecoilSpeed);
		float Diff = RecoverRecoil - RecoveryAmount;
		GetWorld()->GetFirstPlayerController()->AddPitchInput(Diff); // 실제 카메라 회전 적용

		RecoverRecoil = RecoveryAmount;
	}
	//탄퍼짐 회복
	if (CurrentSpread - MinSpread > 1) {
		CurrentSpread = FMath::Clamp(CurrentSpread - (SpreadRecovery * DeltaTime), MinSpread, MaxSpread);
	}



}

void AFirearmBase::Fire()
{
	if (CanFire()) {
		FireBullet();
		ConsumeAmmo();
		PlayFireEffect();
		PlayFireSound();
		RecoilEffect();
	}

	if(!CheckAmmo()){
		Reload();
	}
}

void AFirearmBase::Reload()
{
	if (bReloading == false) {
		bReloading = true;
		GEngine->AddOnScreenDebugMessage(
			0, 1, FColor::White,
			FString::Printf(TEXT("StartReload")));
		GetWorldTimerManager().SetTimer(
			TimerReloadDelayHandle,
			[this]() {
				bReloading = false;
				CurrentAmmo = MaxAmmo;
				GEngine->AddOnScreenDebugMessage(
					0, 1, FColor::White,
					FString::Printf(TEXT("Reload Done")));
			},
			ReloadTime,
			false
		);
	}
	

}
bool AFirearmBase::CanFire()
{
	if (bCanShot == false)
		return false;
	return CheckAmmo();
}

void AFirearmBase::FireBullet()
{
	//prjectile 스폰시켜 날리기
	if (!ProjectileClass)
		return;

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FVector TargetVector = OwnerCharacter->GetCameraTargetLocation();

	FVector SpawnLocation = FirePoint->GetComponentLocation();

	const FVector FireDirection = (TargetVector - SpawnLocation).GetSafeNormal();

	if (FireDirection.IsNearlyZero())
	{
		return;
	}

	const FRotator SpawnRotation = FireDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter;
	SpawnParams.Instigator = OwnerCharacter;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	// BeginPlay에서 서브시스템을 못 가져온 경우를 대비해 다시 가져옵니다.
	if (!PoolSubsystem)
	{
		// 현재 월드에서 오브젝트 풀 서브시스템을 가져옵니다.
		PoolSubsystem = World->GetSubsystem<UObjectPoolSubsystem>();
	}

	// 풀 서브시스템이 없으면 풀 총알을 가져올 수 없으므로 발사를 중단합니다.
	if (!PoolSubsystem)
	{
		// 안전하게 함수 실행을 끝냅니다.
		return;
	}

	for (int32 i = 0; i < Pellets; i++) {
		
		// 서브시스템 Pool방식
		// 풀에서 가져온 총알을 위치, 탄퍼짐 방향, 사거리 기준으로 실제 발사합니다.
		AActor* Bullet = PoolSubsystem->GetPooledActor(FName("Bullet"));
		if (IsValid(Bullet) == false)
			continue;
		ABulletActor* BulletActor = Cast<ABulletActor>(Bullet);
		if (IsValid(BulletActor) == false)
			continue;
		
		FRotator SpreadRotation = SpawnRotation + FRotator{ 
			FMath::FRandRange(-CurrentSpread, CurrentSpread),
			FMath::FRandRange(-CurrentSpread, CurrentSpread),
			0 };
		
		BulletActor->ActivateProjectile(SpawnLocation, SpreadRotation, Range);
		
		// 기존의 Spawn방식
		// ABulletActor* bullet = World->SpawnActor<ABulletActor>(
		// 	ProjectileClass,
		// 	SpawnLocation,
		// 	SpreadRotation,
		// 	SpawnParams
		// );
		// bullet->SetMaxDistance(Range);
		
	}

	bCanShot = false;
	GetWorldTimerManager().SetTimer(
		TimerFireDelayHandle,
		this,
		&AFirearmBase::HandleFireDelay,
		FireRate,
		false
		);
}

void AFirearmBase::ConsumeAmmo()
{
	CurrentAmmo -= 1;
	GEngine->AddOnScreenDebugMessage(
		1, 1, FColor::White,
		FString::Printf(TEXT("Ammo Left : %d"), CurrentAmmo));
}

bool AFirearmBase::CheckAmmo()
{
	return CurrentAmmo >= 1;
}

void AFirearmBase::PlayFireEffect_Implementation()
{

}

void AFirearmBase::PlayFireSound_Implementation()
{

}

void AFirearmBase::RecoilEffect()
{
	//좌우반동
	float hRecoil = FMath::FRandRange(-HorizontalRecoil, HorizontalRecoil);
	GetWorld()->GetFirstPlayerController()->AddYawInput(hRecoil);

	//상하반동
	float vRecoil = FMath::FRandRange(VerticalRecoil/2, VerticalRecoil);
	GetWorld()->GetFirstPlayerController()->AddPitchInput(-vRecoil);

	RecoverRecoil += vRecoil;
}

void AFirearmBase::HandleFireDelay()
{
	GetWorldTimerManager().ClearTimer(TimerFireDelayHandle);
	bCanShot = true;
}
