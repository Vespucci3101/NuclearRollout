// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageableActor.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "DestructableBox.generated.h"

UCLASS()
class OSCARKILOTANGO_API ADestructableBox : public ADamageableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructableBox();
	virtual void ApplyDamage() override;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* BoxSparkleVFX;
};
