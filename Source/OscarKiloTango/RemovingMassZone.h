// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RemovingMassZone.generated.h"

UCLASS()
class OSCARKILOTANGO_API ARemovingMassZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARemovingMassZone();
	void RemoveMass(float DeltaTime);

	/*UPROPERTY(EditAnywhere)
	float SecondsBetweenDecrease = 1.f;*/

	UPROPERTY(EditAnywhere)
	float DecreaseSpeed = 10.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
