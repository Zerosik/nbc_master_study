// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageTypeTest.h"

UDamageTypeTest::UDamageTypeTest()
{
	//물리충격량
	DamageImpulse = 5000.f;
	//무게에 따른 물리?
	bScaleMomentumByMass = true;
	//데미지 발생원인이 world인가? (낙상, 용암 등)
	bCausedByWorld = true;
}
