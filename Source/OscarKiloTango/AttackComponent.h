// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Materials/MaterialInstance.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OSCARKILOTANGO_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttackComponent();

	UPROPERTY(EditAnywhere)
	TArray <UMaterialInstance*> UnlitMaterials;
	UPROPERTY(EditAnywhere)
	TArray <UMaterialInstance*> LitMaterials;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void OnAttack();
	void ApplyDamage();
	bool CanAttack();
	bool IsAttacking();
	void SetMaterials(TArray <UMaterialInstance*> materials);
	FVector GetGroundPos();
	FRotator GetGroundRot();

	// Cooldown time before being able to trigger another attack in seconds.
	UPROPERTY(EditAnywhere)
	float CooldownTime = 0.5f;
	// Attack time in seconds.
	UPROPERTY(EditAnywhere)
	float AttackTime = 0.5f;
	// Visually see the attack range if set to true. (Only for debugging purposes)
	UPROPERTY(EditAnywhere)
	bool DrawDebug = false;
	float m_lastAttackTime = -1.f;

	// Attack distance starting at sphere bounds in cm.
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0"))
	float AttackDistance = 425.f;

	UPROPERTY(BlueprintReadWrite)
	UNiagaraComponent* Shockwave;
	UPROPERTY(BlueprintReadWrite)
	UNiagaraComponent* GroundShockwave;

	bool IsOnCooldown = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
};
