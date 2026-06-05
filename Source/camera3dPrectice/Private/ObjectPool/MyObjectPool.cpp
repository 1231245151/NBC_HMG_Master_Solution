// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool/MyObjectPool.h"

#include "ObjectPool/PooledObject.h"

// Called when the game starts or when spawned
void AMyObjectPool::BeginPlay()
{
	Super::BeginPlay();

    // 스폰 규칙
    FActorSpawnParameters SpawnParams;

    // 액터의 종류
    for (int32 PoolIndex = 0; PoolIndex < PooledObjectData.Num(); ++PoolIndex)
    {
        // 실제 컴포넌트가 들어갈 공간 구조체
        FSingleObjectPool CurrentpoolIndex;

        SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *PooledObjectData[PoolIndex].ActorName));

        // 이름정하는 규칙
        // 내가 생성한 이름을 최대한 사용해달라
        SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

        // 가장 중요한거
        // 스폰될때 한공간에 충돌체가 겹칠 경우 스폰할지, 스폰을 안할지, 다른위치에스폰할지결정
        // 겹쳐도 스폰
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        // 해당 풀의 오브젝트 생성 개수
        for (int32 ObjectIndex = 0; ObjectIndex < PooledObjectData[PoolIndex].Poolsize; ++ObjectIndex)
        {
            // 스폰
            AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolIndex].ActorTemplate, &FVector::ZeroVector,&FRotator::ZeroRotator, SpawnParams);

            // 아웃라이너에서 뜨는 이름 재설정
            SpawnedActor->SetActorLabel(SpawnedActor->GetName());

            // 컴포넌트 만들기
            UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);

            // 컴포넌트를 언리얼에 기능적 등록
            PoolComp->RegisterComponent();

            // 특정 액터의 소유다
            SpawnedActor->AddInstanceComponent(PoolComp);

            // PooledObject의 초기화 함수 호출
            PoolComp->Init(this);

            // 방금 만들어준 컴포넌트를 저장 중
            CurrentpoolIndex.PooledObjects.Add(PoolComp);

            SpawnedActor->SetActorHiddenInGame(true);
            SpawnedActor->SetActorEnableCollision(false);
            SpawnedActor->SetActorTickEnabled(false);
            SpawnedActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        }

        //컴포넌트뭉치를 저장
        Pools.Add(CurrentpoolIndex);

    }
}

AActor* AMyObjectPool::GetPooledActor(FString Name)
{
    // 우리가 저장한 컴포넌트의 갯수를 받아준다.
    int32 PoolCount = Pools.Num();

    // 원하는 액터가 몇번째 인덱스에서 담겨잇나
    int32 CurrentPoolIndex = -1;

    for (int32 i = 0; i < PoolCount; ++i)
    {
        // 찾는 이름과 같다면
        if (PooledObjectData[i].ActorName == Name)
        {
            CurrentPoolIndex = i;
            break;
        }
    }

    // 존재하지않으면 나가기
    if (CurrentPoolIndex == -1)
        return nullptr;

    // 해당 인덱스가 들어있는 컴포넌트 개수
    int32 PoolObjectCount = Pools[CurrentPoolIndex].PooledObjects.Num();

    // 여기 안에서도 어디부터 사용할수 있는지 확인
    int32 FirstAvailable = -1;


    // 순회
    // 찾은 인덱스 안에서  어디부터 사용중인지 판단
    for (int32 i = 0; i < PoolObjectCount; ++i)
    {
        // 안에 내용물(컴포넌트)이 nullptr이 아닐시
        if (Pools[CurrentPoolIndex].PooledObjects[i] != nullptr)
        {
            // 그 컴포넌트가 Active가 아닐 시
            if (!Pools[CurrentPoolIndex].PooledObjects[i]->bIsPoolActive)
            {
                FirstAvailable = i;
                break;
            }
        }
        // 여기왔다? 그놈은 뭔가잘못만들어진놈이니 재생성하라
        else
        {
            // 재생성
            RegenItem(CurrentPoolIndex, i);

            // 이제 잘된 데이터일거니 이거써라
            FirstAvailable = i;
            break;
        }
    }

    // 사용가능한 컴포넌트가 있다.
    if (FirstAvailable >= 0)
    {
        // 만들어진 오브젝트의 컴포넌트를 끌고와서 사용중으로 변경
        UPooledObject* ToReturn = Pools[CurrentPoolIndex].PooledObjects[FirstAvailable];
        ToReturn->bIsPoolActive = true;

        // 회수 예약, 언제 어디서 회수할지모르기에 일단 예약
        OnPoolerCleanup.AddUniqueDynamic(ToReturn, &UPooledObject::RecycleSelf);

        // 활성화 작업
        AActor* ToReturnActor = ToReturn->GetOwner();
        ToReturnActor->SetActorHiddenInGame(false);
        ToReturnActor->SetActorTickEnabled(true);
        ToReturnActor->SetActorEnableCollision(true);

        // 부모를 끊고 최상단으로 간다.
        ToReturnActor->AttachToActor(nullptr, FAttachmentTransformRules::SnapToTargetIncludingScale);


        return ToReturnActor;
    }

    // 사용가능한 컴포넌트가 없다.
    // 추가생성이 가능한옵션이 켜져있지 않으면 종료.
    if (!PooledObjectData[CurrentPoolIndex].bCanGrow)
        return nullptr;

    // bCanGrow가 켜져있음
    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *PooledObjectData[CurrentPoolIndex].ActorName));
    SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FSingleObjectPool CurrentpoolIndex;

    // 스폰
    AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[CurrentPoolIndex].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
    // 아웃라이너에서 뜨는 이름 재설정
    SpawnedActor->SetActorLabel(SpawnedActor->GetName());
    // 컴포넌트 만들기
    UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
    // 컴포넌트를 언리얼에 기능적 등록
    PoolComp->RegisterComponent();
    // 특정 액터의 소유다
    SpawnedActor->AddInstanceComponent(PoolComp);
    // PooledObject의 초기화 함수 호출
    PoolComp->Init(this);
    // 방금 만들어준 컴포넌트를 저장 중
    CurrentpoolIndex.PooledObjects.Add(PoolComp);

    //사용중 처리
    PoolComp->bIsPoolActive = true;

    OnPoolerCleanup.AddUniqueDynamic(PoolComp, &UPooledObject::RecycleSelf);

    return SpawnedActor;
}

void AMyObjectPool::RecycleActor(AActor* PooledActor)
{
    if (UPooledObject* PoolCompRef = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
    {
        RecyclePooledObject(PoolCompRef);
    }

}

void AMyObjectPool::RecyclePooledObject(UPooledObject* PoolCompRef)
{
    // 바인딩 회수
    OnPoolerCleanup.RemoveDynamic(PoolCompRef, &UPooledObject::RecycleSelf);

    // 비 사용중으로 바꾸기
    PoolCompRef->bIsPoolActive = false;

    // 안보이게 만들기
    AActor* ReturnningActor = PoolCompRef->GetOwner();
    ReturnningActor->SetActorHiddenInGame(true);
    ReturnningActor->SetActorEnableCollision(false);
    ReturnningActor->SetActorTickEnabled(false);
    ReturnningActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);


}

void AMyObjectPool::Broadcast_PoolerCleanup()
{
    // 전부지우기용 신호보냄!
    OnPoolerCleanup.Broadcast();
}

// 급히 만들기
void AMyObjectPool::RegenItem(int32 PoolIndex, int32 PositionIndex)
{
    //스폰 준비
    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *PooledObjectData[PoolIndex].ActorName));
    SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    // 스폰
    AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolIndex].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
    // 아웃라이너에서 뜨는 이름 재설정
    SpawnedActor->SetActorLabel(SpawnedActor->GetName());

    UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
    PoolComp->RegisterComponent();
    SpawnedActor->AddInstanceComponent(PoolComp);

    PoolComp->Init(this);

    Pools[PoolIndex].PooledObjects.Insert(PoolComp, PositionIndex);

    SpawnedActor->SetActorHiddenInGame(true);
    SpawnedActor->SetActorEnableCollision(false);
    SpawnedActor->SetActorTickEnabled(false);
    SpawnedActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
