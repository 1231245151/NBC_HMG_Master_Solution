#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Jurnel.generated.h"

UCLASS()
class CAMERA3DPRECTICE_API AJurnel : public AActor
{
	GENERATED_BODY()

public:
	AJurnel();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
	FString JurnelName;

public:
	FString GetJurnelName();

};
