// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorComponent.h"

// Sets default values for this component's properties
UMyActorComponent::UMyActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    MaxHealth = 100.f;
}


// Called when the game starts
void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();

    CurrentHealth = MaxHealth;

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UMyActorComponent::DamageTake);
}


// Called every frame
void UMyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("HP : %f"), CurrentHealth));
}

void UMyActorComponent::DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer)
{
    float FinalDamage = FMath::Min(Damage, CurrentHealth); // 0이하로 안빠지게 최종데미지
    CurrentHealth -= FinalDamage;
    OnHealthDamaged.Broadcast(CurrentHealth, MaxHealth, FinalDamage);
    if (CurrentHealth <= 0.f)
    {
        OnHealthDead.Broadcast(Instigator);
    }
}

