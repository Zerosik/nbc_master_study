// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon/BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Subsystem/ObjectPoolSubsystem.h"
// Sets default values
ABulletActor::ABulletActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 풀에서 꺼내 사용 중일 때 거리 체크를 해야 하므로 Tick 자체는 허용합니다.
	PrimaryActorTick.bCanEverTick = true;
	// 풀에 보관 중인 총알은 Tick이 돌지 않도록 시작 시에는 꺼둡니다.
	PrimaryActorTick.bStartWithTickEnabled = false;

	// --- New Code Start ---
	// Use a simple sphere as the collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	check(CollisionComponent != nullptr);
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABulletActor::OnHit);
	RootComponent = CollisionComponent;

	// Create this projectile's mesh component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	check(ProjectileMesh != nullptr);
	// Attach the sphere component to the mesh component
	ProjectileMesh->SetupAttachment(CollisionComponent);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	check(ProjectileMovement != nullptr);
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Friction = 0.5f;
	ProjectileMovement->ProjectileGravityScale = 0.1f;
	// 풀 생성 시점에 자동으로 날아가지 않도록 ProjectileMovement 자동 활성화를 끕니다.
	ProjectileMovement->bAutoActivate = false;
	// 풀에 보관된 총알의 이동 컴포넌트 Tick을 기본적으로 비활성화합니다.
	ProjectileMovement->SetComponentTickEnabled(false);


}

void ABulletActor::SetMaxDistance(float MaxRange)
{
	MaxDistance = MaxRange;
}

// 풀에서 꺼낸 총알을 실제 발사 상태로 초기화합니다.
void ABulletActor::ActivateProjectile(const FVector& StartLocation, const FRotator& FireRotation, float MaxRange)
{
	// 이번 발사의 최대 이동 거리를 저장합니다.
	SetMaxDistance(MaxRange);

	// 이번 발사의 시작 위치를 거리 체크 기준점으로 저장합니다.
	SpawnLocation = StartLocation;

	// 총알을 발사 위치로 이동합니다.
	SetActorLocation(StartLocation);

	// 총알을 발사 방향으로 회전시킵니다.
	SetActorRotation(FireRotation);

	// 액터 Tick을 켜서 최대 사거리 체크가 동작하게 합니다.
	SetActorTickEnabled(true);

	// 이전 발사에서 남은 속도를 제거합니다.
	ProjectileMovement->StopMovementImmediately();

	// 이동 대상 컴포넌트를 충돌 컴포넌트로 다시 지정합니다.
	ProjectileMovement->SetUpdatedComponent(CollisionComponent);

	// 발사 방향과 속도를 새로 계산해 이동 속도로 넣습니다.
	ProjectileMovement->Velocity = FireRotation.Vector() * ProjectileMovement->InitialSpeed;

	// 이동 컴포넌트 Tick을 켭니다.
	ProjectileMovement->SetComponentTickEnabled(true);

	// ProjectileMovement를 활성화해 지금부터 날아가게 만듭니다.
	ProjectileMovement->Activate(true);
}

// 총알을 풀에 보관할 수 있도록 이동 상태를 정지합니다.
void ABulletActor::DeactivateProjectile()
{
	// 이동 컴포넌트에 남은 속도를 제거합니다.
	ProjectileMovement->StopMovementImmediately();

	// ProjectileMovement를 비활성화해 풀 안에서 움직이지 않게 합니다.
	ProjectileMovement->Deactivate();

	// 이동 컴포넌트 Tick을 꺼서 보관 중 비용을 줄입니다.
	ProjectileMovement->SetComponentTickEnabled(false);

	// 액터 Tick도 꺼서 보관 중 사거리 체크를 멈춥니다.
	SetActorTickEnabled(false);
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();

	// 풀에 생성된 직후에는 발사 상태가 아니므로 이동 컴포넌트를 정지 상태로 둡니다.
	DeactivateProjectile();
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FVector::Dist(GetActorLocation(), SpawnLocation) > MaxDistance)
	{
		// 풀로 돌아가기 전에 이동 컴포넌트를 정지합니다.
		DeactivateProjectile();

		// 현재 월드의 오브젝트 풀 서브시스템을 가져옵니다.
		UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();

		// 서브시스템이 유효할 때만 총알을 풀로 회수합니다.
		if (PoolSubsystem)
		{
			// 사거리를 초과한 총알을 풀로 되돌립니다.
			PoolSubsystem->RecycleActor(this);
		}
		// Destroy(); // 거리 초과 시 파괴
	}
}

void ABulletActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * PhysicsForce, GetActorLocation());
		// 풀로 돌아가기 전에 이동 컴포넌트를 정지합니다.
		DeactivateProjectile();

		// 현재 월드의 오브젝트 풀 서브시스템을 가져옵니다.
		UObjectPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();

		// 서브시스템이 유효할 때만 총알을 풀로 회수합니다.
		if (PoolSubsystem)
		{
			// 충돌한 총알을 풀로 되돌립니다.
			PoolSubsystem->RecycleActor(this);
		}
		// Destroy();
	}
}

