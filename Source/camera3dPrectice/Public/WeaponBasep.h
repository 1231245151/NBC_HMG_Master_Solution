// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "WeaponBasep.generated.h"

UCLASS()
class CAMERA3DPRECTICE_API AWeaponBasep : public AWeaponItem
{
	GENERATED_BODY()
	
public:	
	AWeaponBasep();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> Root;

    //방향 가리키는 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<class UArrowComponent> FirePoint;

    //
    UFUNCTION(BlueprintCallable)
    virtual void Fire();

protected:
    //소모되는 탄약수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AmmoPerFire;

    //남은 탄약수
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    int32 CurrentAmmo;

    //탄약 보유량
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxAmmo;

    // 발사 시 한번에 나가는 총알개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PelletCount;

    // 총알 분산도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SpreadAngle;

    //연사속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RoF;

    //유효사거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Range;

    //데미지양
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamagePerHit;

    // 반동
    float Rebound;

    //쏠수있나
    UPROPERTY(BlueprintReadWrite)
    bool CanFire;

    //연사속도 제어를 위한 핸들
    UPROPERTY(BlueprintReadWrite)
    FTimerHandle TimerFireDelay;

    UFUNCTION()
    void HandleFireDelay();

};
