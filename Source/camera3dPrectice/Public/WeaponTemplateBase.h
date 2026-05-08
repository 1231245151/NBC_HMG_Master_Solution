// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBasep.h"
#include "WeaponTemplateBase.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API AWeaponTemplateBase : public AWeaponBasep
{
	GENERATED_BODY()

public:
    virtual void Fire() override;

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void Reload();

protected:
    // C++ + 블루프린트 하이브리드
    // _Implementation가 붙어야 한다.
    UFUNCTION(BlueprintNativeEvent)
    bool CheckAmmo();

    // 블루 프린트 위임
    //총을 어떻게 쏠지
    UFUNCTION(BlueprintNativeEvent)
    void ProcessFiring();

    //이펙트 실행
    UFUNCTION(BlueprintImplementableEvent)
    void PlayEffects();

    //총알 업데이트
    UFUNCTION(BlueprintNativeEvent)
    void UpdateAmmo();


};
