// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/PooledObject.h"

#include "ObjectPool/MyObjectPool.h"

void UPooledObject::Init(AMyObjectPool* Owner)
{
    bIsPoolActive = false;

    // 오브젝트 풀의 주인
    ObjectPool = Owner;

}

void UPooledObject::RecycleSelf()
{
    // 예시, 총알 맞췃을 시 호출시킴
    ObjectPool->RecyclePooledObject(this);

}

void UPooledObject::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    // 해당 컴포넌트가 삭제되었을 시
    ObjectPool->OnPoolerCleanup.RemoveDynamic(this, &UPooledObject::RecycleSelf);
    Super::OnComponentDestroyed(bDestroyingHierarchy);

}
