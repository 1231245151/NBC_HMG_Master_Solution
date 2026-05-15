// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyActorComponent.generated.h"


// 1대 다수로 블루프린트까지 지원하는 죽었을 때.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthDeadSignature, AController*, Instigator);
// 데미지를 입었을때
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthDamagedSignature, float, NewHealth, float, MaxHealth, float, HealthChange);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAMERA3DPRECTICE_API UMyActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyActorComponent();

    UPROPERTY(BlueprintAssignable)
    FHealthDeadSignature OnHealthDead;
    UPROPERTY(BlueprintAssignable)
    FHealthDamagedSignature OnHealthDamaged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
    float MaxHealth;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
    float CurrentHealth;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UFUNCTION()
    void DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer);


};
