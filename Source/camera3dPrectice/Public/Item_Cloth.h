// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "TestMyInterface.h"
#include "Item_Cloth.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API AItem_Cloth : public AItemBase, public ITestMyInterface
{
	GENERATED_BODY()

public:
    // ITestMyInterface을(를) 통해 상속됨
    virtual void OnFireDetected_Implementation(float Temperature, FVector HitLocation);

private:

protected:
    UPROPERTY(EditAnywhere, Category = "Effects")
    TObjectPtr<class UParticleSystem> FireEffect;
};
