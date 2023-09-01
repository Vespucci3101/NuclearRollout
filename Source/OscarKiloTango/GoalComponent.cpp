// Fill out your copyright notice in the Description page of Project Settings.


#include "GoalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "BallGameInstance.h"
#include "BallGameMode.h"

UGoalComponent::UGoalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGoalComponent::BeginPlay()
{
	Super::BeginPlay();
	BallGameMode = Cast<ABallGameMode>(UGameplayStatics::GetGameMode(this));
	GoalWasFired = false;
	StartedEndingAnim = false;
	BallGameMode->HasOverlapGoal = false;
	LerpAlpha = 0;
	PawnIsVisible = true;
	StartAnimation = false;
	SplashWasPlayed = false;
}

void UGoalComponent::FireGoalEvent()
{
	if (BallGameMode && !BallGameMode->HasOverlapGoal)
	{
		BallGameMode->CallEndLevel(true);
		BallGameMode->HasOverlapGoal = true;
	}
}

void UGoalComponent::StopBall(ABall_Pawn* ballPawn)
{
	if (StartedEndingAnim) return; //Make sure to not call twice

	BallPawn = ballPawn;
	SplashWasPlayed = false;

	StartedEndingAnim = true;
	Velocity = ballPawn->StaticMesh->GetPhysicsLinearVelocity();

	ballPawn->RemoveAllPlayerMoveSpeed();
	ballPawn->SetActorEnableCollision(false);

	UFunction* func = BallPawn->GetClass()->FindFunctionByName(FName("StopBallPawnSFX"));
	if (func) BallPawn->ProcessEvent(func, NULL);

	FireGoalEvent();
}

void UGoalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!StartedEndingAnim) return;

	if (PawnIsVisible) {
		SetAnimationVelocity(DeltaTime);
		PlaySplashIfNeeded();
		return;
	}
}

void UGoalComponent::SetAnimationVelocity(float DeltaTime)
{
	FVector pawnLocation = BallPawn->StaticMesh->GetComponentLocation();
	FVector desiredLocation = GetComponentLocation() + FVector(0, 0, -200.f);

	DesiredVelocity = (desiredLocation - pawnLocation);
	DesiredVelocity = DesiredVelocity.GetSafeNormal() * Speed;

	LerpAlpha += DeltaTime * VelocityLerpSpeed;
	if (LerpAlpha > 1.f) LerpAlpha = 1.f;

	FVector nextLocation = pawnLocation + FMath::Lerp(Velocity, DesiredVelocity, LerpAlpha) * DeltaTime;
	BallPawn->SetActorLocation(nextLocation);

	if ((nextLocation - desiredLocation).Length()<10.f)
	{
		BallPawn->StaticMesh->SetVisibility(false);
		BallPawn->StaticMesh->SetEnableGravity(false);
		BallPawn->RemoveAllPlayerMoveSpeed();

		PawnIsVisible = false;
		StartAnimation = true;
	}
}

void UGoalComponent::PlaySplashIfNeeded()
{
	if (SplashWasPlayed) return;
	float pawnOriginalRadius = 80.f;

	FVector pawnLocation = BallPawn->GetActorLocation();
	FVector capsuleLocation = GetComponentLocation();
	bool isUnderneathLiquid = (pawnLocation.Z - BallPawn->GetRootComponent()->GetRelativeScale3D().X * pawnOriginalRadius) <= capsuleLocation.Z;
	if (isUnderneathLiquid && SplashVFX)
	{
		FVector newLocation = SplashVFX->GetComponentLocation();
		FVector dir = pawnLocation - capsuleLocation;
		dir.Z = 0;
		if (dir.Length() <= CapsuleRadius)
		{
			newLocation.X = pawnLocation.X;
			newLocation.Y = pawnLocation.Y;
		}
		else
		{
			dir = dir.GetSafeNormal() * CapsuleRadius;
			newLocation.X = capsuleLocation.X + dir.X;
			newLocation.Y = capsuleLocation.Y + dir.Y;
		}
		SplashVFX->SetWorldLocation(newLocation);
		SplashVFX->Activate();
		SplashWasPlayed = true;
	}
}
