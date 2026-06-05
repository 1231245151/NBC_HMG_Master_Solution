#include "Data/TestMyActor.h"

#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


ATestMyActor::ATestMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


void ATestMyActor::BeginPlay()
{
	Super::BeginPlay();

    //동기
    //LoadWithSoftPtr();

    //비동기
    LoadWithSoftPtr2();
}


void ATestMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 동기로드 방식
void ATestMyActor::LoadWithSoftPtr()
{
    if (MySoftMesh.IsNull())
        return;
    
    UStaticMesh* FinalMesh = nullptr;

    // 로드가 되엇을 시
    if (MySoftMesh.IsValid())
    {
        FinalMesh = MySoftMesh.Get();
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("이미로드됨. 즉시 참조"));
    }
    // 로드가 안되어있을 시
    else
    {
        // 동기 로드
        // 메인스레드에서 로드하고있음
        FinalMesh = MySoftMesh.LoadSynchronous();
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("메모리에 없음. 처음부터 로드 시행"));
    }

    if (FinalMesh)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("성공 : %s"), *FinalMesh->GetName()));
    }

    // 해당코드의 결과
    // 에디터에서 첫 실행은 로드실행
    // 그 후 에 실행시 이미로드됨이 뜸
    // 그 이유는 해당 에디터에서 메모리에 이미올라온 상태이기 때문

}

// 비동기로드 방식
void ATestMyActor::LoadWithSoftPtr2()
{
    if (MySoftMesh.IsNull())
        return;

    // 로드가 되엇을 시
    if (MySoftMesh.IsValid())
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("이미로드됨. 즉시 참조"));
        MyOnLoadCompleted();
    }
    // 로드가 안되어있을 시
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("메모리에 없음. 처음부터 로드 시행"));

        FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

        Streamable.RequestAsyncLoad(MySoftMesh.ToSoftObjectPath(),
            FStreamableDelegate::CreateUObject(this, &ATestMyActor::MyOnLoadCompleted));
    }

}

void ATestMyActor::MyOnLoadCompleted()
{
    UStaticMesh* FinalMesh = MySoftMesh.Get();

    if (FinalMesh)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("성공 : %s"), *FinalMesh->GetName()));
    }
}

