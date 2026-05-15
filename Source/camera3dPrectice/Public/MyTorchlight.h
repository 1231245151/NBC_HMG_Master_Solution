// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTorchlight.generated.h"

// C++ 형태로 1대1만 지원한다
// DECLARE_DELEGATE

// C++ 1대 다수
// DECLARE_MULTYCAST

// 1대1형태 블루프린트까지 지원
// DECLARE_DYNAMIC

// 1대 다수로 블루프린트까지 지원
// DECLARE_DYNAMIC_MULTICAST

// 바인드 되기 전까지 1다이트도 차지않는다. 사용시 느려짐
// DECLARE_SPARSE

// 반환값 / 매개변수
// 파라미터가 하나다.
// DECLARE_DELEGATE_OneParam
// 반환값이 있고, 파라미터가 세개다.
// DECLARE_DELEGATE_RetVal_ThreeParams

// MULTICAST는 반환값을 지원하지 않는다.

// 블루프린트까지 지원하고 1대1이며 리턴값이 있고, 두개의 파라미터를 가진다.
// DECLARE_DYNAMIC_RetVal_TwoParams


//------------------------------------------------------------------------------

// 바인딩!!!
// 기다리다가 호출되면 이 함수를 실행시켜줘

// C++전용은 AddUObjet 바인딩
// 블루프린트까지 허용은 AddDynamic  장점은 블루프린트 이벤트 디스패쳐와 연동된다.

// 싱글 1대1 대응 -> Bind
// 멀티 1대 다수 -> Add

// 앞부분 Bind/Add
// 뒷부분 UObject/Dynamic

// 오브젝트, SharedPtr, Lamda, Static, UFUNCTION
// 오브젝트       : 델리게이트.BindUObject(객체, &UMyObject::함수);
// 스마트포인터   : 델리게이트.BindSP(객체, &UMyObject::함수);
// 람다           : 델리게이트.BindLamda([](){});
// 스태틱         : 델리게이트.BindStatic(객체, &UMyObject::함수);
// UFUNCTION      : 델리게이트.BindUFunction(객체,TEXT("함수이름"));

// 싱글 C++ -> BindUObject
// 멀티 C++ -> AddUObject
// 싱글 Dynamic -> BindDynamic
// 멀티 Dynamic -> AddDynamic

// 블루프린트와 연동하는 다이나믹은 UFUNCTION()을 붙여야한다.

//-------------------------------------------------------------------

// 신호를 줘야한다(예시로 보스가죽는부분 = 나죽었다)
// 싱글 1대1대응 = Execute();
// 멀티 1대 다수 = Broadcast();

// 싱글은 바인딩이 꼭 되어야 한다.(크래시남)
// 싱글 캐스트는 반드시 Execute전에 IsBound를 넣어야한다.
// if(MySingleDelegate.IsBound())
// { MySingleDelegate.Execute(); }


// 블루프린트로 받아올 경우
// 이쪽에서 객체를 만들고, 그 객체를 통해서 블루프린트 델리게이트를 만들어줌

//UPROPERTY(BlueprintAssignable)
//FDeath OnDeath

UCLASS()
class CAMERA3DPRECTICE_API AMyTorchlight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyTorchlight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere)
    TArray<TWeakObjectPtr<AActor>> Items;

    // 액터 컴포넌트 -> Health, HP

};
