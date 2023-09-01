// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerBallGui.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerBallGui::APlayerBallGui()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerBallGui::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerBallGui::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerBallGui::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float APlayerBallGui::GetPlayerSpeed()
{
	return GetVelocity().Length();
}