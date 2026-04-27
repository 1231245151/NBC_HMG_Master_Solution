// Copyright Epic Games, Inc. All Rights Reserved.

#include "camera3dPrecticeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Engine/DamageEvents.h"
#include "FireDamageType.h"
#include "Jurnel.h"
#include "Item.h"
#include "WeaponItem.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// Acamera3dPrecticeCharacter

Acamera3dPrecticeCharacter::Acamera3dPrecticeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	CurrentWeapon = nullptr;
}

//////////////////////////////////////////////////////////////////////////
// Custom Input


float Acamera3dPrecticeCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 데미지 타입 가져옴
	const UFireDamageType* FireDamage = DamageEvent.DamageTypeClass->GetDefaultObject<UFireDamageType>();

	// 파이어 데미지타입이 맞다면
	if (FireDamage)
	{
		//화상
		ActualDamage *= (1.f + FireDamage->ArmorPenetration);

		// 화상 효과

		UE_LOG(LogTemp, Warning, TEXT("ByWorld Damage "));
	}

	//HP -= ActualDamage;

	return ActualDamage;
}

void Acamera3dPrecticeCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 아이템 추가 또는 칭호추가
	ActorGet(OtherActor);
}

void Acamera3dPrecticeCharacter::ActorGet(AActor* TargetActor)
{
	if (!TargetActor)
        return;

	// 1. 칭호 처리
	if (AJurnel* Jurnel = Cast<AJurnel>(TargetActor))
	{
		FString Name = Jurnel->GetJurnelName();
		if (!JurnelSet.Contains(Name))
		{
			JurnelSet.Add(Name);
			TargetActor->Destroy(); // 칭호는 실체가 필요 없으니 파괴
			MYSCREENLOG("칭호 획득: %s", *Name);
		}
		return;
	}

	// 2. 아이템 처리
	if (AItem* Item = Cast<AItem>(TargetActor))
	{
		// 칭호 자격 확인
		FString Require = Item->GetRequireJurnel();
		if (!Require.IsEmpty() && !JurnelSet.Contains(Require))
		{
			MYSCREENLOG("자격 미달! [%s] 칭호가 필요함", *Require);
			return;
		}

		// 포인터 그대로 키로 등록
		ItemDatabase.Add(TargetActor, Item->GetItemInfo());
		ItemInventory.Add(TargetActor);

		if (CurrentWeapon)
		{
			// 나중에 꺼내기 위해 숨기기
			TargetActor->SetActorHiddenInGame(true);
			TargetActor->SetActorEnableCollision(false);
		}
		else
		{
			// 바로 장착하기
			ItemInfoPrint(FInputActionValue(), 0);
		}

		MYSCREENLOG("%s 줍기 완료", *Item->GetItemName())
	}

	return;
}

void Acamera3dPrecticeCharacter::ItemInfoPrint(const FInputActionValue& value, int32 _value)
{
	// 유효한 슬롯인지 확인
    if (!ItemInventory.IsValidIndex(_value))
    {
        MYSCREENLOG("%d 칸의 이이템은 존재하지않습니다.", _value+1);
        return;
    }

    // 이미 해당 무기를 끼고있나 확인
    if (CurrentWeapon == ItemInventory[_value])
    {
        MYSCREENLOG("해당 이이템은 이미 장착중입니다.");
        return;
    }

	// 기존 무기가 있다면 다시 숨기기
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon->SetActorEnableCollision(false);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	// 새 무기 꺼내기
	CurrentWeapon = ItemInventory[_value];

	if (CurrentWeapon)
	{
		// 보이게 설정
		CurrentWeapon->SetActorHiddenInGame(false);
		CurrentWeapon->SetActorEnableCollision(true);

        // 캐릭터에 부착
        CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
        // 회전, 이동
        CurrentWeapon->SetActorLocation(GetActorLocation() + GetActorForwardVector() * 50.f);
        CurrentWeapon->SetActorRotation(GetActorRotation());

		// 해당 액터의 정보 출력
		if (ItemDatabase.Contains(CurrentWeapon))
		{
			FString WeaponInfo = ItemDatabase[CurrentWeapon];
			MYSCREENLOG("현재 무기 정보: %s", *WeaponInfo);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void Acamera3dPrecticeCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void Acamera3dPrecticeCharacter::BeginPlay()
{
    Super::BeginPlay();

    if(GetCapsuleComponent())
        GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &Acamera3dPrecticeCharacter::OnOverlapBegin);
}

void Acamera3dPrecticeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &Acamera3dPrecticeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &Acamera3dPrecticeCharacter::Look);

		// 숫자키 입력 바인딩

		EnhancedInputComponent->BindAction(
				NumpedAction1,
				ETriggerEvent::Started,
				this,
				&Acamera3dPrecticeCharacter::ItemInfoPrint, 0
			);
	

		EnhancedInputComponent->BindAction(
				NumpedAction2,
				ETriggerEvent::Started,
				this,
				&Acamera3dPrecticeCharacter::ItemInfoPrint, 1
			);
		

		EnhancedInputComponent->BindAction(
				NumpedAction3,
				ETriggerEvent::Started,
				this,
				&Acamera3dPrecticeCharacter::ItemInfoPrint, 2
			);
		

		EnhancedInputComponent->BindAction(
				NumpedAction4,
				ETriggerEvent::Started,
				this,
				&Acamera3dPrecticeCharacter::ItemInfoPrint, 3
			);

		EnhancedInputComponent->BindAction(
				NumpedAction5,
				ETriggerEvent::Started,
				this,
				&Acamera3dPrecticeCharacter::ItemInfoPrint, 4
			);

		EnhancedInputComponent->BindAction(
				NumpedAction6,
				ETriggerEvent::Started,
				this,
				&Acamera3dPrecticeCharacter::ItemInfoPrint, 5
			);

        EnhancedInputComponent->BindAction(
            MouseLeftClick,
            ETriggerEvent::Started,
            this,
            &Acamera3dPrecticeCharacter::Fire
        );
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void Acamera3dPrecticeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void Acamera3dPrecticeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void Acamera3dPrecticeCharacter::Fire(const FInputActionValue& Value)
{
    if (!CurrentWeapon)
        return;

    // 무기 발사
    AWeaponItem* Weapon = Cast<AWeaponItem>(CurrentWeapon);
    Weapon->Shoot();
}
