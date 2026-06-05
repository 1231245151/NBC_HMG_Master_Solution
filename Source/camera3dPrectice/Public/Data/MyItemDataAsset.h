// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API UMyItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    // 아이템을 뽑기 위해서 id필요

    // 에셋 매니저가 이 파일을 식별할 수 있도록 넘겨주는 id
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(ItemType, GetFName());
    }

    // 예시로 Weapon:Sword_1로 로드
    // Weapon은 카테고리를 엔진에서 설정함

    // 초기화 예시 Weapon:Sword

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FPrimaryAssetType ItemType;


    // struct처럼 활용
    // 무거운 에셋들

    // meta = (AssetBundles : 그룹으로 만듬. 필요없는거까지 만드는거 방지
    UPROPERTY(EditAnywhere, Category = "Visual", meta = (AssetBundles = "Mesh"))
    TSoftObjectPtr<USkeletalMesh> ItemMesh;

    UPROPERTY(EditAnywhere, Category = "Visual", meta = (AssetBundles = "Charactor"))
    TSoftObjectPtr<USkeletalMesh> ItemMesh2;

    UPROPERTY(EditAnywhere, Category = "Stats")
    float AttackPower;

};
