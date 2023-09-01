// Fill out your copyright notice in the Description page of Project Settings.


#include "BallGameMode.h"
#include "Ball_Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "CheckpointActor.h"
#include "BallGameInstance.h"

ABallGameMode::ABallGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	TriggerEndTimerHandle = FTimerHandle();
}

void ABallGameMode::CallGameOver(bool bWonGame)
{
	UBallGameInstance* GameInstance = Cast<UBallGameInstance>(UGameplayStatics::GetGameInstance(this));
	UGameplayStatics::LoadStreamLevel(this, FName("GameOver"), true, true, FLatentActionInfo());
	UGameplayStatics::OpenLevel(this, FName("GameOver"), false);
	UGameplayStatics::UnloadStreamLevel(this, *GameInstance->LevelNames[GameInstance->LevelIndex], FLatentActionInfo(), true);
}

void ABallGameMode::CallEndLevel(bool bLevelFinished)
{
	UBallGameInstance* GameInstance = Cast<UBallGameInstance>(UGameplayStatics::GetGameInstance(this));
	// Fix for disabling the crash when starting a level not from MainMenu
	/*if (GameInstance->CheckpointDestroyed.Num() <= 0)
	{
		UGameplayStatics::OpenLevel(this, *GetWorld()->GetName(), false);
		return;
	}*/

	/*TArray<AActor*> CheckpointsLeft;
	UpdateCheckpoints(CheckpointsLeft);*/

	if (bLevelFinished) {
		CalculateScore();
		GameInstance->LevelIndex++;
		Cast<UBallGameInstance>(GetWorld()->GetGameInstance())->GetScores(true);
		// TODO: play ending anim
		//GetWorldTimerManager().SetTimer(TriggerEndTimerHandle, this, &ABallGameMode::LoadScoresMap, 5.f, false);
		GameInstance->HasPlayedCinematic = false;
	}
	else {
		GameInstance->NumberOfLives--;
		if (GameInstance->NumberOfLives <= 0)
		{
			CallGameOver(false);
			return;
		}

		//TTuple<int, int> LastCheckpoint = GameInstance->GetLastCheckpoint();
		
		// If no checkpoints have been kept and the player dies, the first level has to be loaded
		/*if (LastCheckpoint.Key == 0 && LastCheckpoint.Value == 0)
		{
			GameInstance->PlayerScore = 0;
			GameInstance->LevelIndex = 0;
		}
		else
		{
			GameInstance->PlayerScore = LastCheckpoint.Value;
			GameInstance->LevelIndex = LastCheckpoint.Key;
		}*/
		
		// Load the checkpoints menu
		if (GameInstance->LevelIndex < GameInstance->LevelNames.Num()) {
			UGameplayStatics::LoadStreamLevel(this, *GameInstance->LevelNames[GameInstance->LevelIndex], true, true, FLatentActionInfo());
			UGameplayStatics::OpenLevel(this, *GameInstance->LevelNames[GameInstance->LevelIndex], false);
			if (GameInstance->LevelIndex != 0)
				UGameplayStatics::UnloadStreamLevel(this, *GameInstance->LevelNames[GameInstance->LevelIndex - 1], FLatentActionInfo(), true);
		}
		
	}
}

void ABallGameMode::LoadScoresMap()
{
	UBallGameInstance* GameInstance = Cast<UBallGameInstance>(UGameplayStatics::GetGameInstance(this));
	GameInstance->NumberOfLives = 3;
	UGameplayStatics::LoadStreamLevel(this, FName("ScoresMap"), true, true, FLatentActionInfo());
	UGameplayStatics::OpenLevel(this, FName("ScoresMap"), false);
	UGameplayStatics::UnloadStreamLevel(this, *GameInstance->LevelNames[GameInstance->LevelIndex], FLatentActionInfo(), true);
}

void ABallGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CountDown();

	UBallGameInstance* GameInstance = Cast<UBallGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!HasFinished) DisplayScore = (NCollectibles * CollectibleScoreModifier);

	if (m_ballPawn) {
		Mass = m_ballPawn->GetCurrentMass();
		NCollectibles = m_ballPawn->GetCollectiblesCount();
	}
}

void ABallGameMode::CountDown()
{
	if (!TimerStarted || IsTimeout) return; 
	if (TimeLeft >= 0) TimeLeft -= GetWorld()->GetDeltaSeconds();
	else {
		TimeLeft = 0.f;
		IsTimeout = true;
		UFunction* func = GetClass()->FindFunctionByName(FName("TriggerEventLightsOutAndStop"));
		if (func) ProcessEvent(func, NULL);
		if (m_ballPawn) m_ballPawn->DisableInputs();
		//CallEndLevel(false); is now called in the BP after lights out
	}
	CalculateTimeScore();
}

void ABallGameMode::StartTimer()
{
	TimerStarted = true;
}

void ABallGameMode::StopTimer()
{
	TimerStarted = false;
}

void ABallGameMode::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABallGameMode::CountDown, 1.f, false);
	m_ballPawn = Cast<ABall_Pawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	IsTimeout = false;
}

void ABallGameMode::CalculateScore()
{
	UBallGameInstance* GameInstance = Cast<UBallGameInstance>(UGameplayStatics::GetGameInstance(this));
	CalculateTimeScore();
	GameInstance->PlayerScore += ((NCollectibles * CollectibleScoreModifier) + GameInstance->LevelBonus[GameInstance->LevelIndex]) * TimerScoreModifier;
}

void ABallGameMode::UpdateCheckpoints(TArray<AActor*>& outCheckpoints)
{
	UBallGameInstance* GameInstance = Cast<UBallGameInstance>(UGameplayStatics::GetGameInstance(this));
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpointActor::StaticClass(), outCheckpoints);

	if (outCheckpoints.Num() > 0) GameInstance->AddCheckpoint(GameInstance->LevelIndex, GameInstance->PlayerScore);
	else if (GameInstance->LevelIndex < GameInstance->LevelNames.Num()) GameInstance->CheckpointDestroyed[GameInstance->LevelIndex] = true;
}

void ABallGameMode::CalculateTimeScore()
{
	if (TimeLeft > 50) TimerScoreModifier = 6;
	if (TimeLeft > 40 && TimeLeft <= 50) TimerScoreModifier = 5;
	if (TimeLeft > 30 && TimeLeft <= 40) TimerScoreModifier = 4;
	if (TimeLeft > 20 && TimeLeft <= 30) TimerScoreModifier = 3;
	if (TimeLeft > 10 && TimeLeft <= 20) TimerScoreModifier = 2;
	if (TimeLeft > 0 && TimeLeft <= 10) TimerScoreModifier = 1;

	if (Previous_TimerModifier != TimerScoreModifier)
	{
		Previous_TimerModifier = TimerScoreModifier;
		IsTimerModifierChanging = true;
	}
}