// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    // 에셋 매니저에서 식별할 타입 이름 (예: "Weapon")
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FPrimaryAssetType ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float AttackDamage;

    // 무거운 에셋은 SoftObjectPtr로 선언해서 로드 시점을 제어합니다.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
    TSoftObjectPtr<USkeletalMesh> ItemMesh;

    // ID를 반환하는 함수 오버라이드 (Asset Manager가 식별할 때 사용)
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(ItemType, GetFName());
    }

};
