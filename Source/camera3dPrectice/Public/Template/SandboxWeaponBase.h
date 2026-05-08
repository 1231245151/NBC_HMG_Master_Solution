// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBasep.h"
#include "SandboxWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API ASandboxWeaponBase : public AWeaponBasep
{
	GENERATED_BODY()
public:
    virtual void Fire() override;

    UFUNCTION(BlueprintImplementableEvent)
    void SandboxFire();

    UFUNCTION(BlueprintCallable)
    void Reload();

protected:
    // 총알체크
    UFUNCTION(BlueprintCallable)
    bool CheckAmmo();

    // 총쏘기
    UFUNCTION(BlueprintCallable)
    void LinetraceOneShot(FVector Direction);

    // 사운드
    UFUNCTION(BlueprintCallable)
    void PlaySound(USoundBase* Sound);

    // 총알 업데이트 담당
    UFUNCTION(BlueprintCallable)
    void UpdateAmmo();
};
