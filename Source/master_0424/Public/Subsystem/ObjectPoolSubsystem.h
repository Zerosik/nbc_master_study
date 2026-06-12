// Fill out your copyright notice in the Description page of Project Settings.

// 헤더 파일의 중복 포함을 방지합니다.
#pragma once

#include "CoreMinimal.h"
#include "ObjectPolling/PooledObjectData.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectPoolCleanupSignature);

// 풀 하나가 보관하는 컴포넌트 목록을 리플렉션 가능한 구조체로 선언합니다.
USTRUCT(BlueprintType)
struct FObjectPoolBucket
{
	// 언리얼 리플렉션 시스템에 이 구조체를 등록합니다.
	GENERATED_BODY()

	// 풀에 생성된 액터들의 UPooledObject 컴포넌트를 보관합니다.
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<class UPooledObject>> PooledObjects;
};

UCLASS()
class MASTER_0424_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// 서브시스템이 생성될 때 내부 상태를 초기화합니다.
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 서브시스템이 종료될 때 풀에 남은 참조를 정리합니다.
	virtual void Deinitialize() override;

	// 외부에서 전달한 풀 설정 목록으로 풀을 생성합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void InitializePools(const TArray<FPooledObjectData>& NewPooledObjectData);

	// 이름으로 풀에서 사용 가능한 액터를 가져옵니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetPooledActor(FName PoolName);

	// FString 기반 블루프린트 호출을 FName 기반 풀 조회로 연결합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetPooledActorByString(const FString& PoolName);

	// 액터에 붙어 있는 UPooledObject 컴포넌트를 찾아 풀로 돌려보냅니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecycleActor(AActor* PooledActor);

	// 전달받은 풀 컴포넌트를 사용하지 않는 상태로 되돌립니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecyclePooledObject(class UPooledObject* PoolCompRef);

	// 현재 사용 중인 풀 오브젝트들에게 자기 회수 요청을 브로드캐스트합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void BroadcastPoolerCleanup() const;

	// 모든 풀 데이터를 비우고 액터 참조를 정리합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ClearPools();

	// 사용 중인 오브젝트를 한 번에 회수하기 위한 이벤트입니다.
	UPROPERTY(BlueprintAssignable, Category = "Object Pool")
	FObjectPoolCleanupSignature OnPoolerCleanup;

private:
	// 생성할 풀의 설정값을 서브시스템 내부에 저장합니다.
	UPROPERTY()
	TArray<FPooledObjectData> PooledObjectData;

	// 풀별로 생성된 오브젝트 컴포넌트 목록을 저장합니다.
	UPROPERTY()
	TArray<FObjectPoolBucket> Pools;

	// 풀 이름에 해당하는 인덱스를 찾아 반환합니다.
	int32 FindPoolIndex(FName PoolName) const;

	// 지정한 풀 인덱스에 새 액터와 풀 컴포넌트를 생성합니다.
	UPooledObject* CreatePooledObject(int32 PoolIndex);

	// 액터를 풀 안에 보관되는 비활성 상태로 전환합니다.
	void SetActorStored(AActor* ActorToStore);

	// 액터를 게임에서 사용할 수 있는 활성 상태로 전환합니다.
	void SetActorActive(AActor* ActorToUse);
};
