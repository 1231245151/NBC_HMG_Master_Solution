// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ObjectPool/PooledObjectData.h"
#include "ObjectPoolDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API UObjectPoolDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FPooledObjectData> PoolDataArray;
};
