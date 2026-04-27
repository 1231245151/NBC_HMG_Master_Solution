// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMyPlayerController::AMyPlayerController()
	:InputMappingContext(nullptr)
	, MoveAction(nullptr)
    , NumpedAction1(nullptr)
    , NumpedAction2(nullptr)
    , NumpedAction3(nullptr)
    , NumpedAction4(nullptr)
    , NumpedAction5(nullptr)
    , NumpedAction6(nullptr)
{
}

void AMyPlayerController::BeginPlay()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}
