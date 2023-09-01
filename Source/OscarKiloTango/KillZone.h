// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KillZone.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class OSCARKILOTANGO_API UKillZone : public UBoxComponent
{
	GENERATED_BODY()
public:
	UKillZone();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
private:
	FTimerHandle GoalTimerHandle;
	void FireGoalEvent();

	UPROPERTY(EditAnywhere)
	float WaitUntilRestart = 1.f;

	class ABallGameMode* BallGameMode;
};
