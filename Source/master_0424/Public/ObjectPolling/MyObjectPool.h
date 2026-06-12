// Fill out your copyright notice in the Description page of Project Settings.

// 헤더 파일의 중복 포함을 방지합니다.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectPolling/PooledObjectData.h"
#include "Subsystem/ObjectPoolSubsystem.h"
#include "MyObjectPool.generated.h"

// 레벨에 배치해서 풀 설정을 서브시스템에 등록하는 브리지 액터입니다.
UCLASS()
class MASTER_0424_API AMyObjectPool : public AActor
{
	// 언리얼 리플렉션 시스템에 이 클래스를 등록합니다.
	GENERATED_BODY()

public:
	// 기본 생성자에서 틱 사용 여부를 설정합니다.
	AMyObjectPool();

protected:
	// 게임 시작 시 설정된 풀 데이터를 서브시스템에 등록합니다.
	virtual void BeginPlay() override;

public:
	// 현재 액터에 설정된 풀 데이터를 월드 서브시스템에 등록합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void InitializeObjectPoolSubsystem();

	// 기존 블루프린트 호출을 유지하기 위해 문자열 이름으로 풀 액터를 가져옵니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* GetPooledActor(FString Name);

	// 기존 블루프린트 호출을 유지하기 위해 액터를 서브시스템으로 회수합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecycleActor(AActor* PooledActor);

	// 기존 블루프린트 호출을 유지하기 위해 풀 컴포넌트를 서브시스템으로 회수합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void RecyclePooledObject(class UPooledObject* PoolCompRef);

	// 기존 블루프린트 호출을 유지하기 위해 전체 회수 이벤트를 서브시스템에 요청합니다.
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void Broadcast_PoolerCleanup() const;

	// 이 액터가 BeginPlay에서 서브시스템에 넘겨줄 풀 생성 설정입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	TArray<FPooledObjectData> PooledObjectData;

	// 현재 월드의 오브젝트 풀 서브시스템을 반환합니다.
	UFUNCTION(BlueprintPure, Category = "Object Pool")
	UObjectPoolSubsystem* GetObjectPoolSubsystem() const;
};
