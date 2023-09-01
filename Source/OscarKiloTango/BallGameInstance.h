// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Templates/Tuple.h"
#include "HttpModule.h"
#include "Dom/JsonObject.h"
#include "Score.h"
#include "Interfaces/IHttpResponse.h"
#include "BallGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class OSCARKILOTANGO_API UBallGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite)
	int NumberOfLives = 3;

	UPROPERTY(BlueprintReadWrite)
	int PlayerScore = 0;

	UPROPERTY(BlueprintReadWrite)
	int LevelIndex = 0;

	UPROPERTY(BlueprintReadWrite)
	bool IsAttackTutorialDone = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsItemTutorialDone = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsSizeTutorialDone = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsCameraTutorialDone = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsGamePaused = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsWaitingForServerData = false;

	UPROPERTY(BlueprintReadWrite)
	bool HasPlayedCinematic = false;
	
	//Position of player in leaderboard. 11 means isn't in top 10.
	UPROPERTY(BlueprintReadOnly)
	int PlayerPosition = -1;

	UPROPERTY(BlueprintReadWrite)
	TArray<bool> CheckpointDestroyed;

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> LevelNames;

	UPROPERTY(BlueprintReadOnly)
	TArray<FScore> TopScores;

	UPROPERTY(BlueprintReadOnly)
	TArray<int> LevelBonus;

	UFUNCTION(BlueprintCallable)
	void InitializeDestroyedCheckpoints();

	UFUNCTION(BlueprintCallable)
	bool IsPlaythroughDone();

	UFUNCTION(BlueprintCallable)
	void ResetCheckpointDestroyed(int idx);

	UFUNCTION(BlueprintCallable)
	void ResetAttributes();

	UFUNCTION(BlueprintCallable)
	void PostScore(FString name);

	UFUNCTION(BlueprintCallable)
	void GetScores(bool AddPlayer);

	void InitializeLevelsName();
	void AddCheckpoint(int idx, int PlayerScore);
	TTuple<int, int> GetLastCheckpoint();

	UPROPERTY(BlueprintReadWrite)
	bool IsAutoCamera = false;
private:
	TArray<TTuple<int, int>> m_checkpointsHistory;

	//FString CURL = "https://firestore.googleapis.com/v1/projects/ballsserv/databases/(default)/documents/top-scores";
	FString CURL = "https://us-central1-ballsserv.cloudfunctions.net/https";

	void OnGetResponseReceived(FHttpResponsePtr Response);
	void SetPlayerNameOnBoard(FString name);
	void AddPlayerOnBoard();

	bool AddPlayerToScoreboard = false;
};
