// Fill out your copyright notice in the Description page of Project Settings.


#include "SizeComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
USizeComponent::USizeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USizeComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->InputComponent->BindAxis(TEXT("Scale"), this, &USizeComponent::Scale);
	BallPawn = Cast<ABall_Pawn>(GetOwner());
	Root = GetOwner()->GetRootComponent();
	BaseScale = Root->GetRelativeScale3D().X;
	Sign = 0;
	InputsAreBlocked = false;
}

void USizeComponent::Scale(float Value)
{
	if (InputsAreBlocked) return;
	Sign = Value;
}

void USizeComponent::BlockInputsAndScale(bool KeepCurrentScale)
{
	InputsAreBlocked = true;
	float currentScale = Root->GetRelativeScale3D().X;
	if (currentScale == BaseScale || KeepCurrentScale) Sign = 0;
	else Sign = Root->GetRelativeScale3D().X > BaseScale ? -1 : 1;
}

// Called every frame
void USizeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Sign == 0) return;
	if (InputsAreBlocked)
	{
		FVector newScale = Root->GetRelativeScale3D() + EndLevelSizeSpeed * DeltaTime * Sign;
		newScale = Sign == -1? FVector(FMath::Max(newScale.X, BaseScale)) : FVector(FMath::Min(newScale.X, BaseScale));
		Root->SetRelativeScale3D(newScale);
		if (newScale.X == BaseScale) Sign = 0;
	}
	else
	{
		AdjustMeshProperties(DeltaTime, 1.f, Sign);
	}
}

bool USizeComponent::CanApplyIncrease(int sign)
{
	return (sign < 0 && Root->GetRelativeScale3D().X > MinScale) || (sign > 0 && Root->GetRelativeScale3D().X < MaxScale);
}

void USizeComponent::AdjustMeshProperties(float DeltaTime, float speedIncrease, int sign)
{
	if (!CanApplyIncrease(sign)) return;
	float incr = DeltaTime * speedIncrease * sign;
	FVector newScale = Root->GetRelativeScale3D() + SizeSpeed * incr;
	newScale = FVector(FMath::Min(newScale.X, MaxScale));
	newScale = FVector(FMath::Max(newScale.X, MinScale));
	Root->SetRelativeScale3D(newScale);
	// Mass.
	BallPawn->CurrentMass += MassSpeed * incr;
	BallPawn->StaticMesh->SetMassOverrideInKg(NAME_None, BallPawn->CurrentMass);

	// Adjust Camera with the scale of the ball
	BallPawn->SpringArm->TargetArmLength += TargetArmLenghtIncrement * incr;
	BallPawn->CameraSpringArm->TargetArmLength += TargetCameraArmLenghtIncrement * incr;
	BallPawn->testCam2 -= (0.04f * sign);

	// This part is to make sure that the ball is moving smoothly when gaining or losing size
	// The values are hardcoded sorry
	// BallPawn->MoveSpeed += MoveSpeedModifier * incr;
	BallPawn->Camera->SetWorldRotation(FRotator(BallPawn->Camera->GetComponentRotation().Pitch - CameraPitchIncrement * incr, BallPawn->Camera->GetComponentRotation().Yaw, BallPawn->Camera->GetComponentRotation().Roll));
}

