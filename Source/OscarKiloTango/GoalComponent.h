// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Ball_Pawn.h"
#include "GoalComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OSCARKILOTANGO_API UGoalComponent : public UCapsuleComponent
{
	GENERATED_BODY()
public:
	UGoalComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StopBall(ABall_Pawn* ballPawn);

	UPROPERTY(BlueprintReadWrite)
	bool StartAnimation = false;

	UPROPERTY(BlueprintReadWrite)
	UNiagaraComponent* SplashVFX;
protected:
	virtual void BeginPlay() override;
private:
	void FireGoalEvent();
	void SetAnimationVelocity(float DeltaTime);
	void PlaySplashIfNeeded();
	UPROPERTY(EditAnywhere)
	class ABallGameMode* BallGameMode;
	bool GoalWasFired = false;

	bool StartedEndingAnim = false;
	ABall_Pawn* BallPawn;
	FVector Velocity;
	FVector DesiredVelocity;
	FVector AngularVelocity; //not used anymore

	UPROPERTY(EditAnywhere)
	float VelocityLerpSpeed = 5.f;
	//float Deceleration = 10.f;
	float Speed = 800.f;
	float LerpAlpha = 0;

	bool PawnIsVisible = true;
	bool SplashWasPlayed = false;
};
