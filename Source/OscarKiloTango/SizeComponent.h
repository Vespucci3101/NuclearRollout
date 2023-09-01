// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ball_Pawn.h"
#include "Components/ActorComponent.h"
#include "SizeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OSCARKILOTANGO_API USizeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USizeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AdjustMeshProperties(float DeltaTime, float speedIncrease, int sign);
	UFUNCTION(BlueprintCallable)
	void BlockInputsAndScale(bool KeepCurrentScale);
	UPROPERTY(EditAnywhere)
	float MinScale = 0.8;
	UPROPERTY(EditAnywhere)
	float MaxScale = 5.f;

protected:
	virtual void BeginPlay() override;

private:
	void Scale(float value);
	bool CanApplyIncrease(int sign);

	//Scale
	UPROPERTY(EditAnywhere)
	float SizeSpeed = 7.f;
	UPROPERTY(EditAnywhere)
	float EndLevelSizeSpeed = 10.f;

	//Mass
	UPROPERTY(EditAnywhere)
	float MassSpeed = 17.5f;

	//Move speed
	UPROPERTY(EditAnywhere)
	float MoveSpeedModifier = 10.f;

	//Camera
	UPROPERTY(EditAnywhere)
	float TargetArmLenghtIncrement = 200.f;
	UPROPERTY(EditAnywhere)
	float TargetCameraArmLenghtIncrement = 3500.f;
	UPROPERTY(EditAnywhere)
	float CameraPitchIncrement = 2.f;

	USceneComponent* Root;
	ABall_Pawn* BallPawn;
	int Sign = 0;
	bool InputsAreBlocked = false;
	float BaseScale;
};
