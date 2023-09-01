// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage.h"
#include "DrawDebugHelpers.h"

// Sets default values
AStage::AStage()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStage::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAxis(TEXT("JoystickX"), this, &AStage::JoystickX);
	GetWorld()->GetFirstPlayerController()->InputComponent->BindAxis(TEXT("JoystickY"), this, &AStage::JoystickY);

	m_lastInput = FVector2D();
	m_ball = GetWorld()->GetFirstPlayerController()->GetPawn();
	check(m_ball);
	m_camera = Cast<UCameraComponent>(m_ball->GetDefaultSubobjectByName("Camera")); //TODO: fix + beau
}

// Called every frame
void AStage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(m_camera && m_ball);

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), GetInputRotator(), DeltaTime, rotationSpeed));
}

void AStage::JoystickX(float value)
{
	m_lastInput.X = value;

	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("X = %f"), value));
}

void AStage::JoystickY(float value)
{
	m_lastInput.Y = -value;

	GEngine->AddOnScreenDebugMessage(2, 5.f, FColor::Red, FString::Printf(TEXT("Y = %f"), value));
}

FRotator AStage::GetInputRotator()
{
	FVector fowardVector = FVector(FVector2D(m_camera->GetComponentRotation().RotateVector(FVector::ForwardVector)).GetSafeNormal(), 0);

	FVector rotationAxis = fowardVector * m_lastInput.Y + FRotator(0, -90.f, 0).RotateVector(fowardVector) * m_lastInput.X;
	rotationAxis = rotationAxis.GetSafeNormal();
	rotationAxis = FRotator(0, -90.f, 0).RotateVector(rotationAxis);
	rotationAxis *= FMath::DegreesToRadians(maxRotationAngle) * (FMath::Abs(m_lastInput.X) + FMath::Abs(m_lastInput.Y)) / 2;

	return FRotator(FQuat::MakeFromRotationVector(rotationAxis));
}

