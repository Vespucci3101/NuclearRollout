// Fill out your copyright notice in the Description page of Project Settings.


#include "BallCapsuleComponent.h"


UBallCapsuleComponent::UBallCapsuleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBallCapsuleComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBallCapsuleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UBallCapsuleComponent::GroundCheck()
{
	bool IsGround = false;
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
	{
		IsGround = Actor->ActorHasTag("Ground");
		if (IsGround) {
			return true;
		}
	}
	return IsGround;
}

AActor* UBallCapsuleComponent::GroundCheckHit(bool& result)
{
	result = false;
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors)
	{
		result = Actor->ActorHasTag("Ground");
		if (result) {
			IsGrounded = true;
			return Actor;
		}
	}
	IsGrounded = false;
	return nullptr;
}


