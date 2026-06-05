// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/MyDataAssetTestActor.h"
#include "Data/MyItemDataAsset.h"

// Sets default values
AMyDataAssetTestActor::AMyDataAssetTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyDataAssetTestActor::BeginPlay()
{
	Super::BeginPlay();

    StartLoading();
}

// Called every frame
void AMyDataAssetTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyDataAssetTestActor::StartLoading()
{
    // id설정은 필수
    if (!ItemToLoad.IsValid())
        return;

    UAssetManager& AssetManager = UAssetManager::Get();

    // 번들 필터링
    // 난 Mesh필터만해서 가져올거고 나머지(Charactor랑 Stats)는 가져오지않을것이란 뜻
    TArray<FName> Bundle;
    Bundle.Add(FName("Mesh"));

    LoadingHandle = AssetManager.LoadPrimaryAsset(ItemToLoad, Bundle,
        FStreamableDelegate::CreateUObject(this, &AMyDataAssetTestActor::OnLoadFinished, ItemToLoad)
    );

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("아이템 로드 시작"));
}

void AMyDataAssetTestActor::OnLoadFinished(FPrimaryAssetId LoadedId)
{
    // 에셋매니저에게 아이디를 보내 데이터 에셋을 꺼냄
    LoadedItem = Cast<UMyItemDataAsset>(UAssetManager::Get().GetPrimaryAssetObject(LoadedId));

    if (LoadedItem)
    {
        FString Msg = FString::Printf(TEXT("배달 완료 아이템 이름 : %s"), *LoadedItem->ItemMesh.ToString());
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);

        // 실제로 메시를 교체... 수치 적용..~~~~~~~~~
    }

}

