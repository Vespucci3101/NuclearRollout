// Fill out your copyright notice in the Description page of Project Settings.


#include "BackgroundMover.h"
#include "Ball_Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UBackgroundMover::UBackgroundMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBackgroundMover::BeginPlay()
{
	Super::BeginPlay();
	Ball = Cast<ABall_Pawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Actor = GetOwner();
	if (Actor) {
		//StaticMesh = Actor->FindComponentByClass<UStaticMeshComponent>();
		StaticMesh = Cast<UStaticMeshComponent>(Actor->GetRootComponent());
	}
	if (StaticMesh && Ball)
	{
		MeshRotation = StaticMesh->GetComponentRotation();
	}
	MainRotation = StaticMesh->GetComponentRotation();
}


// Called every frame
void UBackgroundMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// this works fine
	// StaticMesh->AddRelativeRotation(FRotator(0.f,-StaticMesh->GetComponentRotation().Yaw, 0.f));
	// if (Ball) StaticMesh->SetWorldLocation(Ball->CameraSpringArm->GetComponentLocation());
}
