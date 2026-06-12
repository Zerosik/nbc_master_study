// Fill out your copyright notice in the Description page of Project Settings.

// 오브젝트 풀 서브시스템 선언을 사용하기 위해 포함합니다.
#include "Subsystem/ObjectPoolSubsystem.h"
// ProjectileMovement를 가진 액터를 풀에 넣을 때 이동을 정지하기 위해 포함합니다.
#include "GameFramework/ProjectileMovementComponent.h"
// 풀 컴포넌트 타입을 사용하기 위해 포함합니다.
#include "ObjectPolling/PooledObject.h"

// 서브시스템 생성 시 부모 초기화와 내부 배열 초기화를 수행합니다.
void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UE_LOG(LogTemp, Warning, TEXT("UObjectPoolSubsystem::Initialize"));
	// 부모 월드 서브시스템의 초기화 로직을 먼저 실행합니다.
	Super::Initialize(Collection);

	// 이전 월드에서 남아 있을 수 있는 풀 설정 배열을 비웁니다.
	PooledObjectData.Empty();

	// 이전 월드에서 남아 있을 수 있는 풀 저장 배열을 비웁니다.
	Pools.Empty();
	
	
}

// 서브시스템 종료 시 풀 참조와 이벤트 바인딩을 정리합니다.
void UObjectPoolSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("UObjectPoolSubsystem::Deinitialize"));
	// 풀에 남은 이벤트와 배열 참조를 먼저 정리합니다.
	ClearPools();

	// 부모 월드 서브시스템의 종료 로직을 실행합니다.
	Super::Deinitialize();
}

// 외부에서 전달받은 설정을 기준으로 풀을 재생성합니다.
void UObjectPoolSubsystem::InitializePools(const TArray<FPooledObjectData>& NewPooledObjectData)
{
	UE_LOG(LogTemp, Warning, TEXT("UObjectPoolSubsystem::InitializePools"));
	// 기존 풀과 이벤트 바인딩을 먼저 정리합니다.
	ClearPools();

	// 새 풀 설정을 서브시스템 내부 배열에 복사합니다.
	PooledObjectData = NewPooledObjectData;

	// 풀 설정 개수만큼 풀 저장 공간을 미리 확보합니다.
	Pools.Reserve(PooledObjectData.Num());

	// 각 풀 설정을 순회하며 초기 풀 오브젝트를 생성합니다.
	for (int32 PoolIndex = 0; PoolIndex < PooledObjectData.Num(); PoolIndex++)
	{
		// 현재 풀에서 생성된 컴포넌트를 저장할 버킷을 만듭니다.
		FObjectPoolBucket CurrentPool;

		// 현재 풀의 목표 개수만큼 배열 공간을 미리 확보합니다.
		CurrentPool.PooledObjects.Reserve(PooledObjectData[PoolIndex].PoolSize);

		// 초기 풀 크기만큼 오브젝트를 생성합니다.
		for (int32 ObjectIndex = 0; ObjectIndex < PooledObjectData[PoolIndex].PoolSize; ObjectIndex++)
		{
			// 지정한 풀 인덱스에 새 오브젝트를 생성합니다.
			UPooledObject* CreatedObject = CreatePooledObject(PoolIndex);

			// 생성에 성공한 컴포넌트만 현재 풀에 저장합니다.
			if (CreatedObject)
			{
				// 생성된 풀 컴포넌트를 현재 풀 버킷에 추가합니다.
				CurrentPool.PooledObjects.Add(CreatedObject);
			}
		}

		// 완성된 풀 버킷을 전체 풀 목록에 추가합니다.
		Pools.Add(CurrentPool);
	}
}

// FName 풀 이름으로 사용 가능한 액터를 찾아 반환합니다.
AActor* UObjectPoolSubsystem::GetPooledActor(FName PoolName)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, 
	FString::Printf(TEXT("UObjectPoolSubsystem::GetPooledActor(%s)"), *PoolName.ToString()));
	
	// 요청한 이름에 해당하는 풀 인덱스를 찾습니다.
	const int32 CurrentPoolIndex = FindPoolIndex(PoolName);

	// 풀 이름을 찾지 못하면 null을 반환합니다.
	if (CurrentPoolIndex == INDEX_NONE)
	{
		// 유효하지 않은 요청이므로 사용할 액터가 없음을 알립니다.
		return nullptr;
	}

	// 현재 풀에 들어 있는 컴포넌트 개수를 저장합니다.
	const int32 PooledObjectCount = Pools[CurrentPoolIndex].PooledObjects.Num();

	// 재사용할 수 있는 컴포넌트 인덱스를 초기화합니다.
	int32 FirstAvailable = INDEX_NONE;

	// 현재 풀의 컴포넌트를 앞에서부터 순회합니다.
	for (int32 ObjectIndex = 0; ObjectIndex < PooledObjectCount; ObjectIndex++)
	{
		// 현재 위치의 풀 컴포넌트를 가져옵니다.
		UPooledObject* PoolComponent = Pools[CurrentPoolIndex].PooledObjects[ObjectIndex];

		// 컴포넌트가 유효하고 사용 중이 아니면 재사용 대상으로 선택합니다.
		if (PoolComponent && !PoolComponent->bIsPoolActive)
		{
			// 재사용 가능한 첫 번째 인덱스를 기록합니다.
			FirstAvailable = ObjectIndex;

			// 가장 앞의 사용 가능 오브젝트를 찾았으므로 순회를 중단합니다.
			break;
		}

		// 배열에는 자리가 있지만 컴포넌트가 사라졌다면 새로 생성합니다.
		if (!PoolComponent)
		{
			// 사라진 컴포넌트를 대체할 새 오브젝트를 생성합니다.
			UPooledObject* RecreatedObject = CreatePooledObject(CurrentPoolIndex);

			// 새로 만든 컴포넌트를 기존 빈 자리에 넣습니다.
			Pools[CurrentPoolIndex].PooledObjects[ObjectIndex] = RecreatedObject;

			// 생성에 성공했다면 이 위치를 반환 대상으로 선택합니다.
			if (RecreatedObject)
			{
				// 재생성된 위치를 사용 가능 인덱스로 기록합니다.
				FirstAvailable = ObjectIndex;

				// 재사용 대상을 찾았으므로 순회를 중단합니다.
				break;
			}
		}
	}

	// 기존 풀 안에서 사용할 수 있는 컴포넌트를 찾았는지 확인합니다.
	if (FirstAvailable != INDEX_NONE)
	{
		// 선택된 풀 컴포넌트를 가져옵니다.
		UPooledObject* ToReturn = Pools[CurrentPoolIndex].PooledObjects[FirstAvailable];

		// 컴포넌트가 비정상이라면 액터를 반환할 수 없습니다.
		if (!ToReturn)
		{
			// 안전하게 null을 반환합니다.
			return nullptr;
		}

		// 풀 컴포넌트를 사용 중 상태로 표시합니다.
		ToReturn->bIsPoolActive = true;

		// 전체 회수 이벤트가 발생했을 때 자기 자신을 회수하도록 바인딩합니다.
		OnPoolerCleanup.AddUniqueDynamic(ToReturn, &UPooledObject::RecycleSelf);

		// 컴포넌트의 소유 액터를 실제 반환 대상으로 가져옵니다.
		AActor* ToReturnActor = ToReturn->GetOwner();

		// 액터를 게임에서 사용할 수 있는 상태로 전환합니다.
		SetActorActive(ToReturnActor);

		// 활성화된 액터를 호출자에게 반환합니다.
		return ToReturnActor;
	}

	// 풀 확장이 금지된 설정이면 새 액터를 만들지 않습니다.
	if (!PooledObjectData[CurrentPoolIndex].bCanGrow)
	{
		// 더 이상 제공할 액터가 없음을 알립니다.
		return nullptr;
	}

	// 확장 가능한 풀이라면 새 풀 컴포넌트를 생성합니다.
	UPooledObject* NewPoolComponent = CreatePooledObject(CurrentPoolIndex);

	// 새 컴포넌트 생성에 실패하면 null을 반환합니다.
	if (!NewPoolComponent)
	{
		// 스폰 실패 상황을 호출자에게 알립니다.
		return nullptr;
	}

	// 새 컴포넌트를 현재 풀 배열 끝에 추가합니다.
	Pools[CurrentPoolIndex].PooledObjects.Add(NewPoolComponent);

	// 새 컴포넌트를 즉시 사용 중 상태로 표시합니다.
	NewPoolComponent->bIsPoolActive = true;

	// 전체 회수 이벤트가 발생했을 때 자기 자신을 회수하도록 바인딩합니다.
	OnPoolerCleanup.AddUniqueDynamic(NewPoolComponent, &UPooledObject::RecycleSelf);

	// 새 컴포넌트의 소유 액터를 가져옵니다.
	AActor* SpawnedActor = NewPoolComponent->GetOwner();

	// 새 액터를 게임에서 사용할 수 있는 상태로 전환합니다.
	SetActorActive(SpawnedActor);

	// 새로 생성된 액터를 호출자에게 반환합니다.
	return SpawnedActor;
}

// FString 이름을 FName으로 변환해 풀 조회 함수로 넘깁니다.
AActor* UObjectPoolSubsystem::GetPooledActorByString(const FString& PoolName)
{
	// 문자열 기반 이름을 FName으로 바꿔 실제 조회 함수를 호출합니다.
	return GetPooledActor(FName(*PoolName));
}

// 액터에서 풀 컴포넌트를 찾아 회수 함수로 넘깁니다.
void UObjectPoolSubsystem::RecycleActor(AActor* PooledActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, 
		FString::Printf(TEXT("UObjectPoolSubsystem::RecycleActor : %s"), *PooledActor->GetName()));
	
	// 유효하지 않은 액터는 회수하지 않습니다.
	if (!PooledActor)
	{
		return;
	}

	// 액터에 붙어 있는 UPooledObject 컴포넌트를 찾습니다.
	if (UPooledObject* PoolCompRef = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
	{
		// 찾은 컴포넌트를 기준으로 실제 회수를 수행합니다.
		RecyclePooledObject(PoolCompRef);
	}
}

// 풀 컴포넌트 하나를 비활성화하고 풀 안으로 되돌립니다.
void UObjectPoolSubsystem::RecyclePooledObject(UPooledObject* PoolCompRef)
{
	// 유효하지 않은 컴포넌트는 회수하지 않습니다.
	if (!PoolCompRef)
	{
		// null 컴포넌트는 처리할 작업이 없으므로 종료합니다.
		return;
	}

	// 전체 회수 이벤트에서 이 컴포넌트의 바인딩을 제거합니다.
	OnPoolerCleanup.RemoveDynamic(PoolCompRef, &UPooledObject::RecycleSelf);

	// 풀 컴포넌트를 사용하지 않는 상태로 표시합니다.
	PoolCompRef->bIsPoolActive = false;

	// 컴포넌트의 소유 액터를 가져옵니다.
	AActor* ReturnActor = PoolCompRef->GetOwner();

	// 액터를 풀 안에 보관되는 비활성 상태로 전환합니다.
	SetActorStored(ReturnActor);
}

// 현재 사용 중인 모든 풀 오브젝트에게 회수를 요청합니다.
void UObjectPoolSubsystem::BroadcastPoolerCleanup() const
{
	// 사용 중인 컴포넌트들이 자기 자신을 회수하도록 이벤트를 호출합니다.
	OnPoolerCleanup.Broadcast();
}

// 풀 배열과 이벤트 바인딩을 모두 비웁니다.
void UObjectPoolSubsystem::ClearPools()
{
	// 모든 회수 이벤트 바인딩을 제거합니다.
	OnPoolerCleanup.Clear();

	// 현재 보관 중인 모든 풀 버킷을 순회합니다.
	for (FObjectPoolBucket& Pool : Pools)
	{
		// 풀 버킷에 저장된 모든 풀 컴포넌트를 순회합니다.
		for (UPooledObject* PoolComponent : Pool.PooledObjects)
		{
			// 유효한 풀 컴포넌트만 액터 정리 대상으로 처리합니다.
			if (PoolComponent)
			{
				// 풀 컴포넌트가 붙어 있는 실제 액터를 가져옵니다.
				AActor* OwnerActor = PoolComponent->GetOwner();

				// 유효한 액터만 월드에서 제거합니다.
				if (OwnerActor)
				{
					// 서브시스템 재초기화 시 이전 풀 액터가 월드에 남지 않도록 파괴합니다.
					OwnerActor->Destroy();
				}
			}
		}
	}

	// 풀에 저장된 컴포넌트 참조 배열을 비웁니다.
	Pools.Empty();

	// 풀 생성 설정 배열을 비웁니다.
	PooledObjectData.Empty();
}

// 이름으로 풀 설정 인덱스를 찾습니다.
int32 UObjectPoolSubsystem::FindPoolIndex(FName PoolName) const
{
	// 저장된 풀 설정을 앞에서부터 순회합니다.
	for (int32 PoolIndex = 0; PoolIndex < PooledObjectData.Num(); PoolIndex++)
	{
		// 현재 설정의 이름을 FName으로 변환합니다.
		const FName CurrentName(*PooledObjectData[PoolIndex].ActorName);

		// 현재 설정 이름이 요청 이름과 같으면 해당 인덱스를 반환합니다.
		if (CurrentName == PoolName)
		{
			// 찾은 풀 인덱스를 반환합니다.
			return PoolIndex;
		}
	}

	// 일치하는 풀이 없으면 INDEX_NONE을 반환합니다.
	return INDEX_NONE;
}

// 풀 설정을 기반으로 새 액터와 UPooledObject 컴포넌트를 생성합니다.
UPooledObject* UObjectPoolSubsystem::CreatePooledObject(int32 PoolIndex)
{
	// 월드 포인터를 가져옵니다.
	UWorld* World = GetWorld();

	// 월드가 없거나 풀 인덱스가 잘못되면 생성하지 않습니다.
	if (!World || !PooledObjectData.IsValidIndex(PoolIndex))
	{
		// 생성할 수 없는 상황이므로 null을 반환합니다.
		return nullptr;
	}

	// 현재 풀 설정을 참조로 가져옵니다.
	const FPooledObjectData& PoolData = PooledObjectData[PoolIndex];

	// 액터 클래스가 지정되지 않았다면 생성하지 않습니다.
	if (!PoolData.ActorTemplate)
	{
		// 스폰할 클래스가 없으므로 null을 반환합니다.
		return nullptr;
	}

	// 액터 스폰 옵션 구조체를 생성합니다.
	FActorSpawnParameters SpawnParams;

	// 풀 이름을 기반으로 액터 이름을 요청합니다.
	SpawnParams.Name = FName(*PoolData.ActorName);

	// 같은 이름이 있어도 엔진이 사용 가능한 이름으로 조정하도록 요청 모드를 사용합니다.
	SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

	// 풀 생성 시 충돌 여부와 관계없이 항상 스폰되도록 설정합니다.
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 지정된 클래스의 액터를 원점에 생성합니다.
	AActor* SpawnedActor = World->SpawnActor<AActor>(PoolData.ActorTemplate, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	// 액터 스폰에 실패했다면 컴포넌트도 만들 수 없습니다.
	if (!SpawnedActor)
	{
		// 스폰 실패를 호출자에게 알립니다.
		return nullptr;
	}

	// 에디터 빌드에서만 아웃라이너 표시 이름을 설정합니다.
#if WITH_EDITOR
	// 에디터 아웃라이너에서 보이는 이름을 실제 액터 이름과 맞춥니다.
	SpawnedActor->SetActorLabel(SpawnedActor->GetName());
	// 에디터 전용 이름 설정 블록을 종료합니다.
#endif

	// 새 액터에 풀 컴포넌트를 생성합니다.
	UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);

	// 컴포넌트 생성에 실패했다면 액터를 파괴하고 null을 반환합니다.
	if (!PoolComp)
	{
		// 사용할 수 없는 액터를 월드에서 제거합니다.
		SpawnedActor->Destroy();

		// 컴포넌트 생성 실패를 호출자에게 알립니다.
		return nullptr;
	}

	// 컴포넌트를 언리얼 컴포넌트 시스템에 등록합니다.
	PoolComp->RegisterComponent();

	// 컴포넌트를 액터의 인스턴스 컴포넌트 목록에 추가합니다.
	SpawnedActor->AddInstanceComponent(PoolComp);

	// 풀 컴포넌트가 이 서브시스템으로 회수 요청을 보낼 수 있도록 초기화합니다.
	PoolComp->Init(this);

	// 생성 직후에는 풀에 보관되는 상태로 액터를 비활성화합니다.
	SetActorStored(SpawnedActor);

	// 생성된 풀 컴포넌트를 반환합니다.
	return PoolComp;
}

// 액터를 숨기고 충돌과 틱을 꺼서 풀에 보관합니다.
void UObjectPoolSubsystem::SetActorStored(AActor* ActorToStore)
{
	// 유효하지 않은 액터는 상태를 바꾸지 않습니다.
	if (!ActorToStore)
	{
		// 처리할 액터가 없으므로 종료합니다.
		return;
	}

	// 회수되는 액터가 다른 부모에 붙어 있다면 분리해 재사용 상태를 깨끗하게 만듭니다.
	ActorToStore->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 액터에 ProjectileMovement가 있다면 풀 안에서 움직이지 않도록 가져옵니다.
	UProjectileMovementComponent* ProjectileMovement = ActorToStore->FindComponentByClass<UProjectileMovementComponent>();

	// ProjectileMovement가 있는 액터만 이동 정지 처리를 수행합니다.
	if (ProjectileMovement)
	{
		// 이전 발사에서 남은 속도를 제거합니다.
		ProjectileMovement->StopMovementImmediately();

		// 이동 컴포넌트를 비활성화해 풀 보관 중 자동 이동을 막습니다.
		ProjectileMovement->Deactivate();

		// 이동 컴포넌트 Tick을 꺼서 보관 중 비용을 줄입니다.
		ProjectileMovement->SetComponentTickEnabled(false);
	}

	// 액터를 게임 화면에서 숨깁니다.
	ActorToStore->SetActorHiddenInGame(true);

	// 비활성 액터의 충돌을 끕니다.
	ActorToStore->SetActorEnableCollision(false);

	// 비활성 액터의 틱을 끕니다.
	ActorToStore->SetActorTickEnabled(false);
}

// 액터를 보이고 충돌과 틱을 켜서 게임에서 사용하게 만듭니다.
void UObjectPoolSubsystem::SetActorActive(AActor* ActorToUse)
{
	// 유효하지 않은 액터는 상태를 바꾸지 않습니다.
	if (!ActorToUse)
	{
		// 처리할 액터가 없으므로 종료합니다.
		return;
	}

	// 풀 보관 중 붙어 있을 수 있는 부모 액터에서 분리합니다.
	ActorToUse->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 액터를 게임 화면에 보이게 합니다.
	ActorToUse->SetActorHiddenInGame(false);

	// 활성 액터의 충돌을 켭니다.
	ActorToUse->SetActorEnableCollision(true);

	// 활성 액터의 틱을 켭니다.
	ActorToUse->SetActorTickEnabled(true);
}
