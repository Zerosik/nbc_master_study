// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletActor.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class MASTER_0424_API ABulletActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletActor();
	 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Physics")
	float PhysicsForce = 100.0f;
 
	// 풀에서 꺼낸 총알을 지정한 위치와 방향으로 발사합니다.
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void ActivateProjectile(const FVector& StartLocation, const FRotator& FireRotation, float MaxRange);

	// 풀로 돌아가는 총알의 이동 상태를 정지시킵니다.
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void DeactivateProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Lifespan")
// 	float ProjectileLifespan = 5.0f;
	// 총알의 최대 이동 거리를 설정합니다.
	void SetMaxDistance(float MaxRange);
 
protected:
	virtual void BeginPlay() override;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Components")
	TObjectPtr<USphereComponent> CollisionComponent;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile | Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
 
public:	
	virtual void Tick(float DeltaTime) override;
private:
	float MaxDistance = 2000.f;
	FVector SpawnLocation = FVector::ZeroVector;
};
