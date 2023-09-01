// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageableActor.h"
#include "GameFramework/Actor.h"
#include "CheckpointActor.generated.h"

UCLASS()
class OSCARKILOTANGO_API ACheckpointActor : public ADamageableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpointActor();
	virtual void ApplyDamage() override;

	// Time given to the player when destroyed in seconds.
	UPROPERTY(EditAnywhere)
	float TimeGivenOnDestroy = 10.f;
};
