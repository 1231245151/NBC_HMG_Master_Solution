// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"


#define MYLOG(format, ...) \
{ \
	UE_LOG(LogTemp, Warning, TEXT(format), ##__VA_ARGS__); \
}

#define MYLOG_ERROR(format, ...) \
{ \
	UE_LOG(LogTemp, Error, TEXT(format), ##__VA_ARGS__); \
}

#define MYSCREENLOG(format, ...)\
{\
    if(GEngine != nullptr)\
	{\
		FString msg = FString::Printf(TEXT(format), ##__VA_ARGS__);\
		GEngine->AddOnScreenDebugMessage(-1, 25.0f, FColor::Blue, msg);\
	}\
}

#define MYSCREENLOG_ERROR(format, ...) \
{ \
   	if (GEngine != nullptr) \
	{ \
		FString msg = FString::Printf(TEXT(format), ##__VA_ARGS__); \
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, msg); \
	} \
}

#define MYGLOG(format, ...) \
{ \
	if (GLog != nullptr) \
	{ \
		FString msg = FString::Printf(TEXT(format), ##__VA_ARGS__); \
		GLog->Log(msg); \
	} \
}

#define MYLOGTRACE(format, ...) \
{ \
    UE_LOG(LogTemp, Warning, TEXT("[%s:%d] " format), \
        TEXT(__FILE__), __LINE__, ##__VA_ARGS__); \
}


UCLASS()
class MYPRECTICEPROJ_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	// 인벤토리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FString> ItemInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<FString, FString> ItemDatabase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSet<FString> JurnelSet;


	UFUNCTION()
	void Move(const FInputActionValue& value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 닿았을 시 이벤트
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	// 칭호 또는 아이템습득 처리
	UFUNCTION()
	void ActorGet(AActor* TargetActor);

    // 숫자패드 입력에 다른 아이템 정보 확인
    void ItemInfoPrint(const FInputActionValue& value, int32 _value);

};
