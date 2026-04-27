// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Jurnel.h"
#include "Item.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();


	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnOverlapBegin);
}

void AMyCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MoveInput = value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 캐스팅 작업
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// GetController 캐스팅 작업
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			// 이동액션 바인딩
			if (PlayerController->MoveAction)
			{
				// IA_Move 액션 키를 "키를 누르고 있는 동안" Move() 호출
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyCharacter::Move
				);
			}

            // 숫자키 입력 바인딩
            if (PlayerController->NumpedAction1)
            {
                EnhancedInput->BindAction(
                    PlayerController->NumpedAction1,
                    ETriggerEvent::Started,
                    this,
                    &AMyCharacter::ItemInfoPrint, 1
                );
            }
            if (PlayerController->NumpedAction2)
            {
                EnhancedInput->BindAction(
                    PlayerController->NumpedAction2,
                    ETriggerEvent::Started,
                    this,
                    &AMyCharacter::ItemInfoPrint, 2
                );
            }
            if (PlayerController->NumpedAction3)
            {
                EnhancedInput->BindAction(
                    PlayerController->NumpedAction3,
                    ETriggerEvent::Started,
                    this,
                    &AMyCharacter::ItemInfoPrint, 3
                );
            }
            if (PlayerController->NumpedAction4)
            {
                EnhancedInput->BindAction(
                    PlayerController->NumpedAction4,
                    ETriggerEvent::Started,
                    this,
                    &AMyCharacter::ItemInfoPrint, 4
                );
            }
            if (PlayerController->NumpedAction5)
            {
                EnhancedInput->BindAction(
                    PlayerController->NumpedAction5,
                    ETriggerEvent::Started,
                    this,
                    &AMyCharacter::ItemInfoPrint, 5
                );
            }
            if (PlayerController->NumpedAction6)
            {
                EnhancedInput->BindAction(
                    PlayerController->NumpedAction6,
                    ETriggerEvent::Started,
                    this,
                    &AMyCharacter::ItemInfoPrint, 6
                );
            }
		}
	}
}

void AMyCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 아이템 추가 또는 칭호추가
	ActorGet(OtherActor);

}

void AMyCharacter::ActorGet(AActor* TargetActor)
{
	// 1. 칭호 액터인지 확인
	if (AJurnel* jurnel = Cast<AJurnel>(TargetActor))
	{
		// 칭호 습득
		FString JurnelName = jurnel->GetJurnelName();
		// 이미 습득한 칭호면 습득불가
		if (JurnelSet.Contains(JurnelName))
		{
            MYSCREENLOG("습득불가!   %s 은/는 이미 가지고 있는 칭호입니다.", *JurnelName)
            MYLOG("습득불가!   %s 은/는 이미 가지고 있는 칭호입니다.", *JurnelName)
		}
		// 없다면 습득
		else
		{
			JurnelSet.Add(JurnelName);
            MYSCREENLOG("칭호 습득!   %s", *JurnelName)
            MYLOG("칭호 습득!   %s", *JurnelName)
            TargetActor->Destroy();
		}


		return;
	}

	// 2. 아이템 엑터인지 확인
	if (AItem* Item = Cast<AItem>(TargetActor))
	{
		// 칭호 조건이 달린 아이템인지 확인
		FString Require = Item->GetRequireJurnel();
		// 조건이 있으며 칭호를 가지지않았을 경우
		if (Require != "" && !JurnelSet.Contains(Require))
		{
            MYSCREENLOG("아이템 습득 불가!   %s 칭호가 없습니다!", *Require)
            MYLOG("아이템 습득 불가!   %s 칭호가 없습니다!", *Require)
			return;
		}

		// 인벤토리에 존재하는 아이템인지확인
		FString ItemName = Item->GetItemName();

		// 가방에 아이템 추가
		ItemInventory.Add(Item->GetItemName());
		// 아이템 정보 등록
		ItemDatabase.Add(ItemName, Item->GetItemInfo());
        MYSCREENLOG("아이템 획득!   %s", *ItemName)
        MYLOG("아이템 획득!   %s", *ItemName)
        TargetActor->Destroy();
		return;
	}

	return;
}

void AMyCharacter::ItemInfoPrint(const FInputActionValue& value, int32 _value)
{
    //if (value.Get<bool>())
    //{
        if (ItemInventory.Num() < _value)
        {
            MYSCREENLOG("%d 칸의 아이템은 존재하지 않습니다.", _value);
            MYLOG("%d 칸의 아이템은 존재하지 않습니다.", _value)
        }
        else
        {
            // 인벤토리에 들어있는 아이템 이름 꺼내기
            FString FoundItemName = ItemInventory[_value - 1];

            // 이름으로 정보 찾아서 출력
            if (ItemDatabase.Contains(FoundItemName))
            {
                MYSCREENLOG("%d 칸의 아이템 정보: %s", _value, **ItemDatabase.Find(FoundItemName));
                MYLOG("%d 칸의 아이템 정보: %s", _value, **ItemDatabase.Find(FoundItemName))
            }
        }
    //}

}

