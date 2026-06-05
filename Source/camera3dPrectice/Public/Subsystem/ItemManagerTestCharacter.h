// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemManagerTestCharacter.generated.h"

UCLASS()
class CAMERA3DPRECTICE_API AItemManagerTestCharacter : public AActor
{
	GENERATED_BODY()
	
public:	

protected:
	virtual void BeginPlay() override;

public:
    /** 테스트를 위해 에디터에서 선택할 아이템 ID */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Test")
    FPrimaryAssetId ItemToEquip;

    /** 아이템 장착 프로세스 시작 */
    UFUNCTION(BlueprintCallable, Category = "Item Test")
    void EquipItem(FPrimaryAssetId ItemId);

    /** 로드가 완료되었을 때 호출될 함수 */
    UFUNCTION()
    void OnEquipReady(FPrimaryAssetId LoadedId);

private:
    //현재 장착된 아이템 데이터를 보관 (선택 사항)
    UPROPERTY()
    class UItemDataAsset* CurrentEquippedItem;

};
