// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASTER_0424_API UPooledObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// 이 컴포넌트를 관리할 오브젝트 풀 서브시스템을 저장합니다.
	void Init(class UObjectPoolSubsystem* Owner);
	// 삭제?
	UFUNCTION(BlueprintCallable)
	void RecycleSelf();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsPoolActive;

private:
	// 회수 요청을 전달할 월드 오브젝트 풀 서브시스템입니다.
	TObjectPtr<class UObjectPoolSubsystem> ObjectPool;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;




};
