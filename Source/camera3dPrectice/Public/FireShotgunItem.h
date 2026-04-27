// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponItem.h"
#include "FireShotgunItem.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API AFireShotgunItem : public AWeaponItem
{
	GENERATED_BODY()

public:
    virtual TSubclassOf<UDamageType> GetDamageType();

protected:
    AFireShotgunItem();

    int32 PelletCount;
    float SpreadAngle;

    virtual void BeginPlay() override;
    virtual bool Shoot() override;
};
