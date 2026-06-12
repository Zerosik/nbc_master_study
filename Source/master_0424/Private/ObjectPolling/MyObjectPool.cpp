// Fill out your copyright notice in the Description page of Project Settings.

// 풀 브리지 액터 선언을 사용하기 위해 포함합니다.
#include "ObjectPolling/MyObjectPool.h"

// 기본 생성자에서 액터 설정을 초기화합니다.
AMyObjectPool::AMyObjectPool()
{
	// 이 브리지 액터는 매 프레임 처리할 일이 없으므로 틱을 비활성화합니다.
	PrimaryActorTick.bCanEverTick = false;
}

// 게임 시작 시 에디터에 설정된 풀 데이터를 서브시스템에 넘깁니다.
void AMyObjectPool::BeginPlay()
{
	// 부모 액터의 BeginPlay 로직을 먼저 실행합니다.
	Super::BeginPlay();

	// 이 액터에 설정된 풀 데이터를 월드 서브시스템에 등록합니다.
	InitializeObjectPoolSubsystem();
}

// 현재 액터가 보유한 풀 설정을 월드 서브시스템에 등록합니다.
void AMyObjectPool::InitializeObjectPoolSubsystem()
{
	// 현재 월드의 오브젝트 풀 서브시스템을 가져옵니다.
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetObjectPoolSubsystem();

	// 서브시스템이 유효할 때만 풀 초기화를 요청합니다.
	if (ObjectPoolSubsystem)
	{
		// 에디터에서 설정한 풀 데이터를 서브시스템에 전달합니다.
		ObjectPoolSubsystem->InitializePools(PooledObjectData);
	}
}

// 기존 API를 유지하면서 서브시스템에서 풀 액터를 가져옵니다.
AActor* AMyObjectPool::GetPooledActor(FString Name)
{
	// 현재 월드의 오브젝트 풀 서브시스템을 가져옵니다.
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetObjectPoolSubsystem();

	// 서브시스템이 없으면 반환할 액터가 없습니다.
	if (!ObjectPoolSubsystem)
	{
		// 호출자에게 실패를 알리기 위해 null을 반환합니다.
		return nullptr;
	}

	// 문자열 이름을 사용해 서브시스템에서 풀 액터를 요청합니다.
	return ObjectPoolSubsystem->GetPooledActorByString(Name);
}

// 기존 API를 유지하면서 액터 회수를 서브시스템에 위임합니다.
void AMyObjectPool::RecycleActor(AActor* PooledActor)
{
	// 현재 월드의 오브젝트 풀 서브시스템을 가져옵니다.
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetObjectPoolSubsystem();

	// 서브시스템이 유효할 때만 액터 회수를 요청합니다.
	if (ObjectPoolSubsystem)
	{
		// 전달받은 액터를 서브시스템의 풀로 되돌립니다.
		ObjectPoolSubsystem->RecycleActor(PooledActor);
	}
}

// 기존 API를 유지하면서 풀 컴포넌트 회수를 서브시스템에 위임합니다.
void AMyObjectPool::RecyclePooledObject(UPooledObject* PoolCompRef)
{
	// 현재 월드의 오브젝트 풀 서브시스템을 가져옵니다.
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetObjectPoolSubsystem();

	// 서브시스템이 유효할 때만 컴포넌트 회수를 요청합니다.
	if (ObjectPoolSubsystem)
	{
		// 전달받은 풀 컴포넌트를 서브시스템의 풀로 되돌립니다.
		ObjectPoolSubsystem->RecyclePooledObject(PoolCompRef);
	}
}

// 기존 API를 유지하면서 전체 회수 이벤트를 서브시스템에 위임합니다.
void AMyObjectPool::Broadcast_PoolerCleanup() const
{
	// 현재 월드의 오브젝트 풀 서브시스템을 가져옵니다.
	UObjectPoolSubsystem* ObjectPoolSubsystem = GetObjectPoolSubsystem();

	// 서브시스템이 유효할 때만 전체 회수를 요청합니다.
	if (ObjectPoolSubsystem)
	{
		// 사용 중인 모든 풀 오브젝트에게 회수 이벤트를 브로드캐스트합니다.
		ObjectPoolSubsystem->BroadcastPoolerCleanup();
	}
}

// 현재 월드에서 오브젝트 풀 서브시스템을 찾아 반환합니다.
UObjectPoolSubsystem* AMyObjectPool::GetObjectPoolSubsystem() const
{
	// 이 액터가 속한 월드를 가져옵니다.
	UWorld* World = GetWorld();

	// 월드가 없으면 서브시스템도 가져올 수 없습니다.
	if (!World)
	{
		// 호출자에게 실패를 알리기 위해 null을 반환합니다.
		return nullptr;
	}

	// 월드에 생성된 오브젝트 풀 서브시스템을 반환합니다.
	return World->GetSubsystem<UObjectPoolSubsystem>();
}
