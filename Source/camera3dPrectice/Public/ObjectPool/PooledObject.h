// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMERA3DPRECTICE_API UPooledObject : public UActorComponent
{
	GENERATED_BODY()

public:	
    void Init(class AMyObjectPool* Owner);

    // 자멸버튼
    UFUNCTION(BlueprintCallable)
    void RecycleSelf();

    UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly)
    bool bIsPoolActive;

private:
    TObjectPtr<class AMyObjectPool> ObjectPool;

    // 자멸버튼
    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};
