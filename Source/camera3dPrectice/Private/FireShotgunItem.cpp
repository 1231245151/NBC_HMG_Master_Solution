#include "FireShotgunItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "FireDamageType.h"


AFireShotgunItem::AFireShotgunItem()
{
    PelletCount = 8;
    SpreadAngle = 5.f;
}

TSubclassOf<UDamageType> AFireShotgunItem::GetDamageType()
{
    return UFireDamageType::StaticClass();
}

void AFireShotgunItem::BeginPlay()
{
}

bool AFireShotgunItem::Shoot()
{
    Super::Shoot();

    // 부모 엑터가 존재해야함
    AActor* MyOwner = GetOwner();
    if (!MyOwner)
        return false;

    // 카메라 정면 좌표 가져오기
    FVector CameraLocation;
    FRotator CameraRotation;
    APlayerController* PC = Cast<APlayerController>(MyOwner->GetInstigatorController());
    // 카메라가 있어야함
    if (PC)
    {
        PC->GetPlayerViewPoint(CameraLocation, CameraRotation);
    }
    else
    {
        return false;
    }
    FVector CameraForward = CameraRotation.Vector();

    // Trace시작
    FVector TraceStart = GetActorLocation();

    // 샷건 쉘
    for (int32 i = 0; i < PelletCount; ++i)
    {
        FVector ShootDir = FMath::VRandCone(CameraForward, FMath::DegreesToRadians(SpreadAngle));
        FVector TraceEnd = TraceStart + (ShootDir * 10000.f);


        // 탄을 발사한다(singletrace)

        // 결과 초기화
        HitResult.Reset();

        // GetWorld()대신 this도 됨
        // 월드, 
        // 시작좌표, 
        // 끝좌표, 
        // Trace채널, 
        // false : 단순콜리전 true : 복합콜리전, 
        // 무시할 액터(안하면 자기자신도 검출)
        // 디버그 옵션
        // 검사결과를 어디다 넣기?
        // 자기자신 검출막기?(여기서도 하는이유는 나중에 다른 액터 붙게되어 검출하는 문제막으려고 안전성확보.)
        // 디버그 라인 색상
        // 막힌 이후의 라인 색상
        TArray<AActor*> ActorsToIgnore;
        ActorsToIgnore.Add(this);
        ActorsToIgnore.Add(MyOwner);

        bool bHit = UKismetSystemLibrary::LineTraceSingle(
            GetWorld(),
            TraceStart,
            TraceEnd,
            UEngineTypes::ConvertToTraceType(ECC_Visibility),
            false, ActorsToIgnore,
            EDrawDebugTrace::ForDuration,
            HitResult, true,
            FLinearColor::Red, FLinearColor::Green
        );
        // 데미지 처리
        AActor* HitActor = HitResult.GetActor();

        // 피해 처리
        if (bHit && HitResult.GetActor())
        {
            // 데미지처리
            // 맞춘 엑터
            // 데미지
            // 맞춘방향
            // Trace결과
            // 가해자 컨트롤러액터는 누구?
            // 어떤 액터가 공격햇어?
            // 데미지클래스
            UGameplayStatics::ApplyPointDamage(
                HitResult.GetActor(),
                10.f,
                ShootDir,
                HitResult,
                PC,
                this,
                GetDamageType()
            );
        }
    }

    // 총기 반동(부모 캐릭터의 컨트롤러로 카메라 x,y축건듬)
    float PitchRecoil = FMath::FRandRange(-0.5f, -2.5f);
    PC->AddPitchInput(PitchRecoil);

    float YawRecoil = FMath::FRandRange(-0.8f, 0.8f);
    PC->AddYawInput(YawRecoil);

    return true;
}
