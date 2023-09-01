// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructableBox.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ADestructableBox::ADestructableBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ADestructableBox::ApplyDamage()
{
	if (BoxSparkleVFX != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BoxSparkleVFX, GetActorLocation());
	}
	Destroy();
}