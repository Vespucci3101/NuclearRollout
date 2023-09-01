// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BackgroundMover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OSCARKILOTANGO_API UBackgroundMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBackgroundMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AActor* Actor;
	UStaticMeshComponent* StaticMesh;
	class ABall_Pawn* Ball;
	FRotator MeshRotation;
	FRotator MainRotation; 


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
