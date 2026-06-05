// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "MyWeaponStruct.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
    GENERATED_BODY()

    // 클래스와 구조체 차이
    // 아무것도 안쓰고 코드를 짤경우
    // Struct 기본 - Public
    // Class 기본 - private

    // Struct사용하면서 빌드하면 더 안정적이다.
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString WeaponName;


    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftClassPtr<AActor> WeaponClass;

};
