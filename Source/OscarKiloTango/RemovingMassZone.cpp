// Fill out your copyright notice in the Description page of Project Settings.


#include "RemovingMassZone.h"
#include "Ball_Pawn.h"
#include "SizeComponent.h"

// Sets default values
ARemovingMassZone::ARemovingMassZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARemovingMassZone::BeginPlay()
{
	Super::BeginPlay();
	/*FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ARemovingMassZone::RemoveMass, SecondsBetweenDecrease, true);*/
}

// Called every frame
void ARemovingMassZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RemoveMass(DeltaTime);
}

void ARemovingMassZone::RemoveMass(float DeltaTime)
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	for (AActor* Actor : Actors) {
		if (Actor->ActorHasTag("Player"))
		{
			USizeComponent* sizeComponent = Cast<USizeComponent>(Actor->GetComponentByClass(USizeComponent::StaticClass()));
			if (sizeComponent == nullptr) continue;
			sizeComponent->AdjustMeshProperties(DeltaTime, DecreaseSpeed, -1);
		}
	}
}

