// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class CAMERA3DPRECTICE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	AItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FString RequireJurnelName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FString ItemInfo;

public:
	FString GetItemName();
	FString GetItemInfo();
	FString GetRequireJurnel();

};
