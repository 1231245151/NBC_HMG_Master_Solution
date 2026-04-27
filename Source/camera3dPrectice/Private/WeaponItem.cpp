#include "WeaponItem.h"

#include "MyTestDamageType.h"

TSubclassOf<UDamageType> AWeaponItem::GetDamageType()
{
    return UMyTestDamageType::StaticClass();
}

AWeaponItem::AWeaponItem()
{
    Rebound = 5.f;
    Damage = 10.f;
}

void AWeaponItem::BeginPlay()
{
    Super::BeginPlay();
}

bool AWeaponItem::Shoot()
{
    return true;
}
