// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/MyWorldSubsystem.h"
#include "Subsystem/TimeWorldSubsystem.h"

bool UMyWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    // 붙어있는곳
    UWorld* World = Cast<UWorld>(Outer);
    bool ISmap;
    if (World)
    {
        ISmap = World->GetMapName().Contains(TEXT("StarterMap"));
    }

    return ISmap;
}

void UMyWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Warning, TEXT("My Subsystem Begin"));

    // 선행 초기화 보장
    // TimeSystem이 먼저 초기화해줘야함
    UTimeWorldSubsystem* TimeSub = Collection.InitializeDependency<UTimeWorldSubsystem>();
    if (TimeSub)
    {
        int32 CurrentHours = TimeSub->GetCurrentHour();
        UE_LOG(LogTemp, Warning, TEXT("Current Time : %d"), CurrentHours);
    }

}

void UMyWorldSubsystem::Deinitialize()
{
    UE_LOG(LogTemp, Warning, TEXT("My Subsystem End"));


    Super::Deinitialize();
}
