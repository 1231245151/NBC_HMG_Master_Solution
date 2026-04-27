// Fill out your copyright notice in the Description page of Project Settings.


#include "Jurnel.h"

// Sets default values
AJurnel::AJurnel()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	JurnelName = "None";
}

FString AJurnel::GetJurnelName()
{
	return JurnelName;
}