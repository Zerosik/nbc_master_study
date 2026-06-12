// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPolling/PooledObject.h"
// 회수 요청을 보낼 월드 서브시스템 타입을 사용하기 위해 포함합니다.
#include "Subsystem/ObjectPoolSubsystem.h"

// 풀 컴포넌트가 자신을 관리할 서브시스템을 기억하도록 초기화합니다.
void UPooledObject::Init(UObjectPoolSubsystem* Owner)
{
	// 새로 생성되거나 재초기화된 컴포넌트를 미사용 상태로 표시합니다.
	bIsPoolActive = false;

	// 회수 요청을 보낼 서브시스템 포인터를 저장합니다.
	ObjectPool = Owner;
}

// 이 컴포넌트가 붙은 액터를 풀로 되돌립니다.
void UPooledObject::RecycleSelf()
{
	// 서브시스템이 유효할 때만 회수 요청을 보냅니다.
	if (ObjectPool)
	{
		// 현재 컴포넌트를 기준으로 액터를 풀에 회수합니다.
		ObjectPool->RecyclePooledObject(this);
	}
}

// 컴포넌트가 파괴될 때 서브시스템 이벤트 바인딩을 제거합니다.
void UPooledObject::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	// 서브시스템이 남아 있을 때만 이벤트 바인딩을 제거합니다.
	if (ObjectPool)
	{
		// 전체 회수 이벤트에서 이 컴포넌트의 콜백을 제거합니다.
		ObjectPool->OnPoolerCleanup.RemoveDynamic(this, &UPooledObject::RecycleSelf);
	}

	// 부모 컴포넌트의 파괴 처리를 이어서 실행합니다.
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}
