#include "Subsystem/ItemManagerTestCharacter.h"

#include "Subsystem/ItemManagerSubsystem.h"
#include "Subsystem/ItemDataAsset.h"

void AItemManagerTestCharacter::BeginPlay()
{
    Super::BeginPlay();

    // 시작하자마자 에디터에서 설정한 아이템 로드를 시도합니다.
    if (ItemToEquip.IsValid())
    {
        EquipItem(ItemToEquip);
    }
}


void AItemManagerTestCharacter::EquipItem(FPrimaryAssetId ItemId)
{
    if (!ItemId.IsValid()) return;

    // 1. 서브시스템을 가져옵니다.
    UItemManagerSubsystem* ItemManager = GetGameInstance()->GetSubsystem<UItemManagerSubsystem>();
    if (!ItemManager) return;

    //2. 바인딩 해줍니다.
    ItemManager->OnItemLoaded.AddUniqueDynamic(this, &AItemManagerTestCharacter::OnEquipReady);



    // 3. 서브시스템에 로드 요청을 보냅니다.
    UE_LOG(LogTemp, Log, TEXT("캐릭터: %s 아이템 로드 요청"), *ItemId.ToString());
    ItemManager->RequestAsyncLoadItem(ItemId);
}

void AItemManagerTestCharacter::OnEquipReady(FPrimaryAssetId LoadedId)
{
    // 우리가 요청했던 그 아이템이 맞는지 확인 (여러 개를 동시에 로드할 때를 대비)
    if (LoadedId != ItemToEquip) return;

    // 4. 로드가 완료되었으니 실제 데이터를 가져옵니다.
    UItemManagerSubsystem* ItemManager = GetGameInstance()->GetSubsystem<UItemManagerSubsystem>();
    UItemDataAsset* Data = ItemManager->GetItemData(LoadedId);

    if (Data)
    {
        CurrentEquippedItem = Data;

        // 실제 장착 처리 (예: 메시 변경, 능력치 적용 등)
        UE_LOG(LogTemp, Warning, TEXT("캐릭터: %s 장착 완료! 공격력: %f"),
            *Data->GetName(), Data->AttackDamage);

        // 로드 완료 후 델리게이트 해제 (필요에 따라 유지하거나 해제)
        ItemManager->OnItemLoaded.RemoveDynamic(this, &AItemManagerTestCharacter::OnEquipReady);
    }
}

