// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageableActor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "DestructibleBumper.generated.h"

/**
 *
 */
UCLASS()
class OSCARKILOTANGO_API ADestructibleBumper : public ADamageableActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		float BumperForceFactor = 0.6f;

	// Sets default values for this actor's properties
	ADestructibleBumper();
	virtual void ApplyDamage() override;
	UPROPERTY(EditAnywhere)
		float BumperMultiplier = 300000;
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* BumperSparkleVFX;

	UFUNCTION()
		void BumpPlayer(AActor* OverlappedActor, AActor* OtherActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
