// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageableActor.h"
#include "DamageableMesh.generated.h"

/**
 * 
 */
UCLASS()
class OSCARKILOTANGO_API ADamageableMesh : public ADamageableActor
{
	GENERATED_BODY()

public:
	ADamageableMesh();
	virtual void ApplyDamage() override;
};
