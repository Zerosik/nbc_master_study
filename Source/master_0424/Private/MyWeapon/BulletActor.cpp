// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon/BulletActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
ABulletActor::ABulletActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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


}

void ABulletActor::SetMaxDistance(float MaxRange)
{
	MaxDistance = MaxRange;
}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();

	SpawnLocation = GetActorLocation();
}

// Called every frame
void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FVector::Dist(GetActorLocation(), SpawnLocation) > MaxDistance)
	{
		Destroy(); // 거리 초과 시 파괴
	}
}

void ABulletActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * PhysicsForce, GetActorLocation());

		Destroy();
	}
}

