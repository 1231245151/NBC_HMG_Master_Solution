#include "Item.h"


AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemName = "None";
	RequireJurnelName = "";
	ItemInfo = "";
}


FString AItem::GetItemName()
{
	return ItemName;
}

FString AItem::GetItemInfo()
{
	return ItemInfo;
}

FString AItem::GetRequireJurnel()
{
	return RequireJurnelName;
}

