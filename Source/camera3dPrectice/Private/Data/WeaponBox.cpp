// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/WeaponBox.h"
#include "Data/MyWeaponStruct.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

// Sets default values
AWeaponBox::AWeaponBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponBox::BeginPlay()
{
	Super::BeginPlay();

    // 아이템 호출
    OpenBox();
}

// Called every frame
void AWeaponBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBox::OpenBox()
{
    if (!WeaponTable)
        return;


    TArray<FWeaponData*> AllWeapons;
    WeaponTable->GetAllRows<FWeaponData>(TEXT(""), AllWeapons);

    if (AllWeapons.Num() == 0)
        return;


    FWeaponData* SelectedWeapon = AllWeapons[FMath::RandRange(0, AllWeapons.Num() - 1)];

    if (SelectedWeapon)
    {
        UClass* LoadedClass = SelectedWeapon->WeaponClass.LoadSynchronous();

        //동기방식
        if (LoadedClass)
        {
            FActorSpawnParameters SpawnParams;
            GetWorld()->SpawnActor<AActor>(LoadedClass, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator, SpawnParams);
        
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("소환완료 : %s"), *SelectedWeapon->WeaponName));
        }


    }

}

void AWeaponBox::OpenBox2()
{
    if (!WeaponTable)
        return;


    TArray<FWeaponData*> AllWeapons;
    WeaponTable->GetAllRows<FWeaponData>(TEXT(""), AllWeapons);

    if (AllWeapons.Num() == 0)
        return;


    FWeaponData* SelectedWeapon = AllWeapons[FMath::RandRange(0, AllWeapons.Num() - 1)];

    if (SelectedWeapon)
    {
        //비동기방식
        FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

        // 원한다면 매개변수를 보내줄수있음
        // 생성단계는 해당 함수에서!
        Streamable.RequestAsyncLoad(SelectedWeapon->WeaponClass.ToSoftObjectPath(),
            FStreamableDelegate::CreateUObject(this, &AWeaponBox::MyOnLoadComplet, SelectedWeapon->WeaponClass, SelectedWeapon->WeaponName));
    }
}

void AWeaponBox::MyOnLoadComplet(TSoftClassPtr<AActor> WeaponClassPtr, FString WeaponName)
{
    // 비동기화 방식으로호출된 함수로 스폰시킨다.
    UClass* LoadedClass = WeaponClassPtr.Get();
    if (LoadedClass)
    {
        FActorSpawnParameters SpawnParams;
        // 큰 에셋이라도 무조건 스폰 옵션
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(LoadedClass, GetActorLocation() + FVector(0, 0, 100), FRotator::ZeroRotator, SpawnParams);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("소환완료 : %s"), *WeaponName));
    }
}

