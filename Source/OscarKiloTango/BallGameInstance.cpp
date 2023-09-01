// Fill out your copyright notice in the Description page of Project Settings.


#include "BallGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBallGameInstance::AddCheckpoint(int Levelindex, int Score)
{
	m_checkpointsHistory.Add(TTuple<int, int>(Levelindex, Score));
}

TTuple<int, int> UBallGameInstance::GetLastCheckpoint()
{
	return (m_checkpointsHistory.Num() > 0) ? m_checkpointsHistory.Pop() : TTuple<int, int>(0, 0);
}

void UBallGameInstance::InitializeDestroyedCheckpoints()
{
	m_checkpointsHistory.Empty();
	CheckpointDestroyed.Empty();
	CheckpointDestroyed.Init(false, 10);
}

void UBallGameInstance::InitializeLevelsName()
{
	// Might be able to do better
	LevelNames.Empty();
	LevelNames.Add("Alpha_0");
	LevelNames.Add("Alpha_1");
	LevelNames.Add("Alpha_6");
	LevelNames.Add("Alpha_8");
	LevelNames.Add("Alpha_9");
	LevelNames.Add("Alpha_7_2");
	LevelNames.Add("Alpha_10");
}

bool UBallGameInstance::IsPlaythroughDone()
{
	return NumberOfLives == 0 || LevelIndex >= LevelNames.Num(); //TODO: check if it works
}

void UBallGameInstance::ResetCheckpointDestroyed(int Levelindex)
{
	for (int i = CheckpointDestroyed.Num() - 1; i >= Levelindex; i--)
	{
		CheckpointDestroyed[i] = false;
	}
}

void UBallGameInstance::ResetAttributes()
{
	LevelIndex = 0;
	PlayerScore = 0;
	NumberOfLives = 3;
	IsAttackTutorialDone = false;
	IsItemTutorialDone = false;
	IsSizeTutorialDone = false;
	IsCameraTutorialDone = false;
	HasPlayedCinematic = false;
	IsGamePaused = false;
	InitializeLevelsName();

	LevelBonus.Empty();
	int levelBonus = 100;
	for (int i = 0; i < LevelNames.Num(); i++) {
		LevelBonus.Add(levelBonus);
		levelBonus += 100;
	}

	//InitializeDestroyedCheckpoints();
}

void UBallGameInstance::PostScore(FString name)
{
	SetPlayerNameOnBoard(name);

	FHttpModule* httpModule = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = httpModule->CreateRequest();
	pRequest->SetVerb(TEXT("POST"));
	pRequest->SetURL(CURL + "/score/" + name + "/" + FString::SanitizeFloat(PlayerScore) + "/" + FString::FromInt(LevelIndex));
	pRequest->ProcessRequest();
}

void UBallGameInstance::GetScores(bool AddPlayer)
{
	TopScores.Empty();
	FHttpModule* httpModule = &FHttpModule::Get();

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> pRequest = httpModule->CreateRequest();
	pRequest->SetVerb(TEXT("GET"));
	pRequest->SetURL(CURL + "/scores");
	IsWaitingForServerData = true;
	AddPlayerToScoreboard = AddPlayer;
	//callback
	pRequest->OnProcessRequestComplete().BindLambda(
		[&](
			FHttpRequestPtr pRequest,
			FHttpResponsePtr pResponse,
			bool connectedSuccessfully) mutable {
				if (connectedSuccessfully && pResponse.Get()->GetResponseCode() == 200) {
					OnGetResponseReceived(pResponse);
				}
				else
				{
					//TODO: message err joli
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("An error occured: %d"), pResponse.Get()->GetResponseCode()));
				}
		});

	pRequest->ProcessRequest();
}

void UBallGameInstance::OnGetResponseReceived(FHttpResponsePtr Response)
{
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	PlayerPosition = 1;
	TopScores.Empty();

	if (FJsonSerializer::Deserialize(Reader, JsonArray)) {

		for(int i = 0; i<JsonArray.Num(); i++)
		{
			auto jsonObject = JsonArray[i]->AsObject();
			float score = jsonObject->GetNumberField("score");
			if (score >= PlayerScore) PlayerPosition++;

			if (TopScores.Num() >= 10) {
				if (AddPlayerToScoreboard) continue;
				else {
					IsWaitingForServerData = false;
					return;
				}
			}

			FString name = jsonObject->GetStringField("name");
			int lastLevel = jsonObject->GetNumberField("lastLevel");

			TopScores.Push(FScore(name, score, lastLevel, TopScores.Num() + 1));
		}
		if (AddPlayerToScoreboard) AddPlayerOnBoard();
	}
	IsWaitingForServerData = false;
}

void UBallGameInstance::SetPlayerNameOnBoard(FString name)
{
	if (PlayerPosition > TopScores.Num() || PlayerPosition-1 < 0) return;
	TopScores[PlayerPosition - 1].name = name;
}

void UBallGameInstance::AddPlayerOnBoard()
{
	if (PlayerPosition > 10 || (TopScores.Num() < 10 && PlayerPosition == TopScores.Num()+1)) {
		TopScores.Push(FScore("YOU", PlayerScore, LevelIndex, PlayerPosition));
	}
	else {
		TopScores.Insert(FScore("YOU", PlayerScore, LevelIndex, PlayerPosition), PlayerPosition-1);
		if(TopScores.Num()>10) TopScores.Pop(true);

		for (int i = PlayerPosition; i < TopScores.Num(); i++)
		{
			TopScores[i].position = i + 1;
		}
	}
}