// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointActor.h"
#include "BallGameMode.h"

// Sets default values
ACheckpointActor::ACheckpointActor()
{

}


void ACheckpointActor::ApplyDamage()
{
	ABallGameMode* gameMode = Cast<ABallGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr) gameMode->TimeLeft += TimeGivenOnDestroy;
	Destroy();
}

