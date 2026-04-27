// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "camera3dPrecticeCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

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

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class Acamera3dPrecticeCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** 키패드 Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* NumpedAction1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* NumpedAction2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* NumpedAction3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* NumpedAction4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* NumpedAction5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* NumpedAction6;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* MouseLeftClick;

	// 인벤토리
	// 칭호 시스템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSet<FString> JurnelSet;
	// 인벤토리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ItemInventory;
	// 아이템 DB
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TMap<AActor*, FString> ItemDatabase;

	// 현재 손에 들고 있는 무기
	UPROPERTY()
	AActor* CurrentWeapon;

public:
	Acamera3dPrecticeCharacter();
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

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

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

    // 무기 사용
    void Fire(const FInputActionValue& Value);
			

protected:

	virtual void NotifyControllerChanged() override;

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

