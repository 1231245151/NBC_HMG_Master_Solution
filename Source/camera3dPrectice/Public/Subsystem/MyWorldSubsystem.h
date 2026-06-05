// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MyWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API UMyWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

    // true :해당 시스템 생성 false : 해당 시스템 생성안함
    // 서버와 클라이언트 분리해서 사용되는 시스템등
    virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

    // 서브시스템 시작
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    // 서브시스템 끝
    virtual void Deinitialize() override;
	
};
