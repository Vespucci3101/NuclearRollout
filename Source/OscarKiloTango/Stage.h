// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "Stage.generated.h"

UCLASS()
class OSCARKILOTANGO_API AStage : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStage();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FRotator GetInputRotator();

	UPROPERTY(EditAnywhere, category = "Gamepad input")
	float minJoystickMovement = 0.2f;

	UPROPERTY(EditAnywhere)
	float rotationSpeed = 15.f;

	UPROPERTY(EditAnywhere)
	float maxRotationAngle = 40.f;

	UCameraComponent* m_camera = nullptr;
	APawn* m_ball = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void JoystickX(float value);
	void JoystickY(float value);

	FVector2D m_lastInput;
};
