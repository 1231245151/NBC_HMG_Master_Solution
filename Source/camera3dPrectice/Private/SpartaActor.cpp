// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MyTestDamageType.h"


// Sets default values
ASpartaActor::ASpartaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpartaActor::BeginPlay()
{
	Super::BeginPlay();
	
	// kismet 사용방식 : 블루프린트, 자유로운 디버깅
	// GetWorld 사용방식 : c언어방식, 성능 부하 적음
	//UKismetSystemLibrary::LineTraceMulti();
	//GetWorld()->LineTraceMultiByChannel();


}

// Called every frame
void ASpartaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//StartSingleTrace();
	//StartMultyTrace();
	StartAsyncTrace();
}

void ASpartaActor::StartSingleTrace()
{
	// GetWorld()대신 this도 됨
	// 월드, 
	// 시작좌표, 
	// 끝좌표, 
	// Trace채널, 
	// false : 단순콜리전 true : 복합콜리전, 
	// 무시할 액터(안하면 자기자신도 검출)
	// 디버그 옵션
	// 검사결과를 어디다 넣기?
	// 자기자신 검출?(여기서도 하는이유는 나중에 다른 액터 붙게되어 검출하는 문제막으려고 안전성확보.)
	// 디버그 라인 색상
	// 막힌 이후의 라인 색상
	FHitResult HitResult;
	TArray <AActor*> ActorsTolgnore;
	ActorsTolgnore.Add(this);
	// Overlap인 액터를 검출이 불가능하다. Block만 검출
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		GetActorLocation(),
		GetActorForwardVector() * 1000.f + GetActorLocation(),
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsTolgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green
	);

}

void ASpartaActor::StartMultyTrace()
{
	// Multi에서는 배열로 결과를 받는다.
	// Overlap인 액터를 검출이 가능하다.
	// 다만, block된 액터 다음에 있는 overlap은 막혀서 검출 불가.
	TArray<FHitResult> HitResult;
	TArray <AActor*> ActorsTolgnore;
	ActorsTolgnore.Add(this);

	UKismetSystemLibrary::LineTraceMulti(
		GetWorld(),
		GetActorLocation(),
		GetActorForwardVector() * 1000.f + GetActorLocation(),
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsTolgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green
	);
}

void ASpartaActor::StartAsyncTrace()
{
	// 멀티로 쓰는지 싱글로 쓰는지 테스트(인식했나안했나, bool값이라 가벼움)인지
	// 시작지점
	// 끝지점
	// Trace채널
	// 옵션
	// 옵션
	// 델리게이트로 넘길 함수 옵션

	// 이게 뭐하는거지
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	// 복합? 단순? 콜리전 검색
	QueryParams.bTraceComplex = false;
	// 이게뭐하는거지
	// WorldDynamic을 ECR_Overlap으로 강제로 변경
	// 우선순위 ignore > overlap > block순이며 역순으로 처리불가니 주의
	FCollisionResponseParams ResponseParam;
	ResponseParam.CollisionResponse.WorldDynamic = ECR_Block;

	FTraceDelegate TraceDelegate;
	TraceDelegate.BindUObject(this, &ASpartaActor::OnAsyncTraceCompleted);



	GetWorld()->AsyncLineTraceByChannel(
		EAsyncTraceType::Multi,
		GetActorLocation(),
		GetActorForwardVector() * 1000.f + GetActorLocation(),
		ECC_Visibility,
		QueryParams,
		ResponseParam,
		&TraceDelegate
	);

}

void ASpartaActor::OnAsyncTraceCompleted(const FTraceHandle& Handle, FTraceDatum& Data)
{
	for (const FHitResult& Hit : Data.OutHits)
	{
		AActor* HitActor = Hit.GetActor();

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("Multi Hit Actor : %s"), *HitActor->GetName()));

		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 20.f, 12, FColor::Green, false, 2.f);
	
			
		// 데미지처리
		// 맞춘 엑터
		// 데미지
		// 맞춘방향
		// 머냐이거?
		// 가해자 컨트롤러액터는 누구?
		// 어떤 액터가 공격햇어? 뭐야이거
		// 데미지클래스 넣어라
		UGameplayStatics::ApplyPointDamage(
			HitActor,
			50.f,
			GetActorForwardVector(),
			Hit,
			GetInstigatorController(),
			this,
			UMyTestDamageType::StaticClass()
		);
	}
}

