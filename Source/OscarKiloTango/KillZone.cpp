// Fill out your copyright notice in the Description page of Project Settings.

#include "KillZone.h"
#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BallGameMode.h"

UKillZone::UKillZone()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UKillZone::BeginPlay()
{
	Super::BeginPlay();
	//BallGameMode = Cast<ABallGameMode>(UGameplayStatics::GetGameMode(this));
}

void UKillZone::FireGoalEvent()
{
	//if (BallGameMode) BallGameMode->CallEndLevel(false);
}

void UKillZone::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors) {
		if (Actor->ActorHasTag("Player"))
		{
			GetOwner()->GetWorldTimerManager().SetTimer(GoalTimerHandle, this, &UKillZone::FireGoalEvent, WaitUntilRestart, false);
		}
	}*/
}