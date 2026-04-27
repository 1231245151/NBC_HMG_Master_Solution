// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyTestDamageType.h"
#include "FireDamageType.generated.h"

/**
 * 
 */
UCLASS()
class CAMERA3DPRECTICE_API UFireDamageType : public UMyTestDamageType
{
	GENERATED_BODY()
	
public:
	UFireDamageType();

	// 화상 지속시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BurnDuration;

	// 방어구 관통
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ArmorPenetration;
};
