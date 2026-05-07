// Fill out your copyright notice in the Description page of Project Settings.

#include "TraceTest.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DamageTypeTest.h"

// Sets default values
ATraceTest::ATraceTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATraceTest::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATraceTest::ShotgunTrace, 1, true);
}

// Called every frame
void ATraceTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//StartSingleTrace();
	//StartAsyncTrace();
}

void ATraceTest::ShotgunTrace()
{
	FVector Start = GetActorLocation();//총구지점같은게 있으면 좋을듯
	FVector ForwardVector = GetActorForwardVector();//조준방향
	float Range = 2000.0f; // 샷건 사거리
	int32 PelletCount = 8; // 샷건 산탄 수
	float SpreadRadius = 10.0f; // 산탄 퍼짐 정도

	//자신 제외
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this); 

	for (int32 i = 0; i < PelletCount; ++i)
	{
		// 최대 SpreadRadius각도로 ForwardVector를 회전한 값 생성(산탄 퍼짐)
		float RandomYaw = FMath::FRandRange(-SpreadRadius, SpreadRadius);
		float RandomPitch = FMath::FRandRange(-SpreadRadius, SpreadRadius);

		FRotator RandomYawPitch = FRotator(RandomPitch, RandomYaw, 0);
		FVector PelletDirection = RandomYawPitch.RotateVector(ForwardVector);

		// 최종 사격지점
		FVector End = Start + (PelletDirection * Range);

		FHitResult HitResult;
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::LineTraceMulti(
			GetWorld(),
			Start,
			End,
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
			false,
			ActorsToIgnore,
			EDrawDebugTrace::ForDuration,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			1.f
		);
	}
}


void ATraceTest::StartSingleTrace()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FHitResult HitResult;
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::LineTraceMulti(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 1000.f,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResults,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		5.f
	);

}

void ATraceTest::StartAsyncTrace()
{
	return;
	FTraceDelegate TraceDelegate;
	TraceDelegate.BindUObject(this, &ATraceTest::OnAsyncTraceCompleted);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.WorldDynamic = ECR_Overlap;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Multi,
		GetActorLocation(),
		GetActorForwardVector() * 1000.f + GetActorLocation(),
		ECC_Visibility,
		QueryParams,
		ResponseParams,
		&TraceDelegate
	);
}

void ATraceTest::OnAsyncTraceCompleted(const FTraceHandle& Handle, FTraceDatum& data) {

	for (const FHitResult& hit : data.OutHits) {
		AActor* HitActor = hit.GetActor();

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Multi Hit Actor : %s"), *HitActor->GetName()));
		
		DrawDebugSphere(GetWorld(), hit.ImpactPoint, 20.f, 12, FColor::Green, false, 2.f);

		UGameplayStatics::ApplyPointDamage(
			HitActor,
			50.f,
			GetActorForwardVector(),
			hit,
			GetInstigatorController(),
			this,
			UDamageTypeTest::StaticClass()
		);
	}
}