// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleBumper.h"
#include "NiagaraFunctionLibrary.h"
#include "Ball_Pawn.h"

// Sets default values
ADestructibleBumper::ADestructibleBumper()
{

}

void ADestructibleBumper::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ADestructibleBumper::BumpPlayer);
}

void ADestructibleBumper::ApplyDamage()
{

}

void ADestructibleBumper::BumpPlayer(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		ABall_Pawn* BallPawn = Cast<ABall_Pawn>(OtherActor);
		BallPawn->StaticMesh->AddForce(-BallPawn->GetVelocity().GetSafeNormal() * (BumperForceFactor * BumperMultiplier), NAME_None, true);
		if (BumperSparkleVFX != nullptr)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BumperSparkleVFX, GetActorLocation() + (BallPawn->GetActorLocation() - GetActorLocation()) / 2);
	}
}