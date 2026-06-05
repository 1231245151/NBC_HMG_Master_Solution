// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestMyActor.generated.h"

UCLASS()
class CAMERA3DPRECTICE_API ATestMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


    TSoftObjectPtr<UStaticMesh> Mesh;
    TSoftClassPtr<AActor> MyActorClass;
    // 소프트 래퍼런스는 동기 비동기 둘다가능
    // 소프트 래퍼런스 -> 동기로드....
    UPROPERTY(EditAnywhere, Category = "Setting")
    TSoftObjectPtr<UStaticMesh> MySoftMesh;

    void LoadWithSoftPtr();


    // 소프트 래퍼런스 -> 비동기로드...
    void LoadWithSoftPtr2();
    void MyOnLoadCompleted();

};
