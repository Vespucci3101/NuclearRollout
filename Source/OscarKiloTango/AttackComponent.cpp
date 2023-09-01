// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include "DamageableActor.h"
#include "Ball_Pawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/EngineTypes.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->InputComponent->BindAction("Attack", IE_Pressed, this, &UAttackComponent::OnAttack);
	IsOnCooldown = false;
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsAttacking()) ApplyDamage();

	if (IsOnCooldown && CanAttack())
	{
		IsOnCooldown = false;
		SetMaterials(LitMaterials);
	}
}

void UAttackComponent::OnAttack()
{
	if (!CanAttack()) return;

	ApplyDamage();
	if (Shockwave && GroundShockwave)
	{
		Shockwave->Activate();
		
		//GroundShockwave->SetRelativeLocation(GetGroundPos());
		//GroundShockwave->SetWorldRotation(GetGroundRot());
		//GroundShockwave->SetRelativeRotation(GetGroundRot())
		//GroundShockwave->Activate();
	}

	SetMaterials(UnlitMaterials);
	IsOnCooldown = true;
	
	m_lastAttackTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}

FVector UAttackComponent::GetGroundPos()
{
	return FVector(0, 0, -100.f * GetOwner()->GetRootComponent()->GetRelativeScale3D().X) + GetOwner()->GetActorLocation();
}

FRotator UAttackComponent::GetGroundRot()
{
	return FRotator::ZeroRotator;
}

void UAttackComponent::ApplyDamage()
{
	float attackRadius = GetOwner()->GetRootComponent()->GetRelativeScale3D().X * AttackDistance;
	TArray<FOverlapResult> overlappingActors;
	FCollisionShape collisionShape;
	collisionShape.SetSphere(attackRadius);

	if(DrawDebug) DrawDebugSphere(GetWorld(), GetOwner()->GetActorLocation(), attackRadius, 100, FColor::Red);

	if (GetWorld()->OverlapMultiByObjectType(overlappingActors, GetOwner()->GetActorLocation(), FQuat::Identity, FCollisionObjectQueryParams(), collisionShape))
	{
		for (FOverlapResult overlap : overlappingActors)
		{
			ADamageableActor* damageable = Cast<ADamageableActor>(overlap.GetActor());
			if (damageable != nullptr)
			{
				damageable->ApplyDamage();
			}
		}
	}
}

bool UAttackComponent::CanAttack()
{
	if (m_lastAttackTime < 0.f) return true;
	return UGameplayStatics::GetRealTimeSeconds(GetWorld()) - m_lastAttackTime > CooldownTime + AttackTime;
}

bool UAttackComponent::IsAttacking()
{
	if (m_lastAttackTime < 0.f) return false;
	return UGameplayStatics::GetRealTimeSeconds(GetWorld()) - m_lastAttackTime < AttackTime;
}

void UAttackComponent::SetMaterials(TArray <UMaterialInstance*> materials)
{
	ABall_Pawn* pawn = Cast<ABall_Pawn>(GetOwner());
	if (materials.Num() != 3 || !pawn) return;
	pawn->StaticMesh->SetMaterial(0, materials[0]);
	pawn->StaticMesh->SetMaterial(1, materials[1]);
	pawn->StaticMesh->SetMaterial(2, materials[2]);
}

