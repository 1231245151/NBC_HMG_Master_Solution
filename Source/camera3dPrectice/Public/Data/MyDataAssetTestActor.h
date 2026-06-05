// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/AssetManager.h" // 1. 필수!

#include "MyDataAssetTestActor.generated.h"

UCLASS()
class CAMERA3DPRECTICE_API AMyDataAssetTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyDataAssetTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    // 2.아이템 id를 가지고 있다가 로드되면 여기에 저장

    // 아이디를 넣어주는곳
    UPROPERTY(EditAnywhere, Category = "ItemTest")
    FPrimaryAssetId ItemToLoad;

    UPROPERTY(VisibleAnywhere, Category = "ItemTest")
    class UMyItemDataAsset* LoadedItem;

    // 3. 최초로 실행되는 함수
    void StartLoading();


    // 4. AssetManager에게 요청시 비동기로 로드될 때 잊어버리지않게 가지고 있기
    // 택배로 비유)
    // 편의점 택배로 주문 : 내가 이거 보내달라고 해야 함
    // 편의점 가서 수령 : 아이디 가지고 가서 내 물건 달라고 해야 함
    TSharedPtr<FStreamableHandle> LoadingHandle;

    void OnLoadFinished(FPrimaryAssetId LoadedId);

};
