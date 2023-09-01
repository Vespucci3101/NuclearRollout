// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BallGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OSCARKILOTANGO_API ABallGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABallGameMode();

	void CallGameOver(bool bWonGame);

	UFUNCTION(BlueprintCallable)
	void CallEndLevel(bool bLevelFinished);

	virtual void Tick(float DeltaSeconds);

	void CountDown();

	UFUNCTION(BlueprintCallable)
	void StartTimer();

	UFUNCTION(BlueprintCallable)
	void StopTimer();

	UFUNCTION(BlueprintCallable)
	void LoadScoresMap();

	UPROPERTY(BlueprintReadOnly)
	float TimeLeft = 60.f;

	UPROPERTY(BlueprintReadOnly)
	float Mass = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NCollectibles = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollectibleScoreModifier = 500.f;

	UPROPERTY(BlueprintReadOnly)
	float TimerScoreModifier = 1000.f;

	UPROPERTY(BlueprintReadWrite)
	float DisplayScore = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool HasFinished = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsTimerModifierChanging = false;

	bool HasOverlapGoal = false;

	int Previous_TimerModifier = 6;

	UPROPERTY(BlueprintReadOnly)
	bool IsTimeout = false;

protected:
	virtual void BeginPlay() override;

private:
	class ABall_Pawn* m_ballPawn;

	void CalculateScore();

	void CalculateTimeScore();

	void UpdateCheckpoints(TArray<AActor*>& outCheckpoints);
	
	bool TimerStarted = false;

	FTimerHandle TriggerEndTimerHandle;
};
