#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponItem.generated.h"


UCLASS()
class CAMERA3DPRECTICE_API AWeaponItem : public AItem
{
	GENERATED_BODY()

public:
    virtual TSubclassOf<UDamageType> GetDamageType();


    virtual bool Shoot();

protected:
    AWeaponItem();

    FHitResult HitResult;

    virtual void BeginPlay() override;

private:
    float Rebound;      // 반동
    float Damage;       // 피해량

	
};
