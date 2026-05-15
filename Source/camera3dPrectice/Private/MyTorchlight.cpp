// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTorchlight.h"
#include "TestMyInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyTorchlight::AMyTorchlight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTorchlight::BeginPlay()
{
	Super::BeginPlay();

    for (const TWeakObjectPtr<AActor>& Item : Items)
    {
        //// cpp에서만 사용가능(블루프린트호환안됨)
        //ITestMyInterface* MyInterface = Cast<ITestMyInterface>(Item.Get());
        //
        //if (MyInterface)
        //{
        //    MyInterface->OnFireDetected(100.f, FVector::ZeroVector);
        //}

        if (UKismetSystemLibrary::DoesImplementInterface(Item.Get(), UTestMyInterface::StaticClass()))
        {
            ITestMyInterface::Execute_OnFireDetected(Item.Get(), 100.f, FVector::ZeroVector);
        }
    }
}

// Called every frame
void AMyTorchlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

