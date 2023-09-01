// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "BallCapsuleComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OSCARKILOTANGO_API UBallCapsuleComponent : public UCapsuleComponent
{
	GENERATED_BODY()
public:
	UBallCapsuleComponent();
	bool GroundCheck();
	AActor* GroundCheckHit(bool& result);
	UPROPERTY(BlueprintReadOnly)
	bool IsGrounded = false;
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:

};
