// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/AssetManager.h"
#include "ItemManagerSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemLoaded, FPrimaryAssetId, AssetId);

UCLASS()
class CAMERA3DPRECTICE_API UItemManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    // 서브 시스템 초기화
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    // 아이템 비동기 로드요청
    UFUNCTION(BlueprintCallable, Category = "ItemManager")
    void RequestAsyncLoadItem(FPrimaryAssetId AssetId);

    // 로드된 아이템 가져오기
    UFUNCTION(BlueprintCallable, Category = "ItemManager")
    class UItemDataAsset* GetItemData(FPrimaryAssetId AssetId);

    // 아이템 메모리 해제
    UFUNCTION(BlueprintCallable, Category = "ItemManager")
    void UnloadItem(FPrimaryAssetId AssetId);

    // 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "ItemManager")
    FOnItemLoaded OnItemLoaded;

private:
    void OnLoadCompleted(FPrimaryAssetId AssetId);

    // 로드된 핸들을 보관하여 가비지 컬렉션을 방지
    TMap<FPrimaryAssetId, TSharedPtr<FStreamableHandle>> LoadingHandles;
};
