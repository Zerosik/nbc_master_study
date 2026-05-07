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
 
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Mesh")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

// 	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile | Lifespan")
// 	float ProjectileLifespan = 5.0f;
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
