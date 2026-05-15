// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyActorComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

    ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    UMyActorComponent* comp = player->FindComponentByClass<UMyActorComponent>();
    if (comp)
    {
        comp->OnHealthDead.AddDynamic(this, &AItemBase::DestroyItem);
    }
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::DestroyItem(AController* _constrollor)
{
    Destroy();
}

