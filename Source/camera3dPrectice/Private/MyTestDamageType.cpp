// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestDamageType.h"

UMyTestDamageType::UMyTestDamageType()
{
	// 물리적으로 주는 충격량
	DamageImpulse = 5000.f;
	// 무게에 따른 밀림을 만든다.
	bScaleMomentumByMass = true;
	// 누가 준건지 모름(환경데미지 등)
	bCausedByWorld = true;
}
