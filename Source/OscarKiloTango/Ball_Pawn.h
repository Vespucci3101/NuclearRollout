// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NiagaraComponent.h"
#include "Ball_Pawn.generated.h"

UENUM(BlueprintType)
enum Items
{
	Boost		UMETA(DisplayName = "BoostPad"),
	Spring		UMETA(DisplayName = "SpringPad"),
};

UCLASS()
class OSCARKILOTANGO_API ABall_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABall_Pawn();

	float CurrentMass = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraSpringArm;
	UPROPERTY(EditAnywhere, Category = "Movements")
	float MoveSpeed = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UFUNCTION(BlueprintImplementableEvent)
	void NotAbleToPlaceItemEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void CreatingItemEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UBallCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SpringArmHolder;

	// Items.
	AActor* Microbot;
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<class AActor> MicrobotClass;
	AActor* ItemHologram;
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<class AActor> ItemHologramClass;
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<class AActor> BoostPadClass;
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TSubclassOf<class AActor> SpringClass;
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	UStaticMesh* BoostPadMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	UStaticMesh* SpringMesh;
	UPROPERTY(EditAnywhere, Category = "Items")
	int ItemSpawnDistance = 400;
	UPROPERTY(EditAnywhere, Category = "Items")
	bool ItemUsed = false;
	// Value added to the scale of the player to bring closer or farther the item.
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0.f, ClampMax = 5.f))
	float ItemScaleDistanceModifier = 1.f;
	
	void UseItem();
	void ToggleCamera();
	
	// Movements
	float XValue = 0.f;
	float YValue = 0.f;
	UPROPERTY(EditAnywhere, Category = "Movements")
	float MaxVelocity = 200.f;
	float CurrentVelocity = 0.f;
	float LastXValue = 0.f;
	float LastYValue = 0.f;
	bool InSlope = false;


	void Move(float Value);
	void Turn(float Value);
	void TurnCamera(float Value);
	void RaiseCamera(float Value);
	float DotProductResult();
	float DotProductCamera();
	FVector GetForceVector(float Value, FVector RefVector);
	bool IsMovingBackward = false;
	bool HasStartedMovingBack = false;
	float ManualCamAngle = 0.f;

	float CalculateCameraSpringArmPitch();
	void RotateSpringArmToFollowSpeed(float DeltaTime);
	void RotateCameraSpringArm(float DeltaTime);
	void SetItemHologramMesh();
	TArray<FVector> GetItemRaycastImpactPoints();

	float HorizontalRotation = 0.f;
	float CameraPitchIncrement = 0.05;
	float VerticalRotationSpringArm = 0.f;
	float CameraSpringArmPitch = 0.f;

	float MaxVerticalAngleForward = 18.f;
	float MaxHorizontalAngle = 30.f;
	float MaxVerticalAngleBackward = 15.f;

	bool IsMicrobotReturning = false;
	float ItemZ = 0.f;
	class UBallGameInstance* GameInstance; 

	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	/*UPROPERTY(EditAnywhere, Category = "Collectible")
	float ScaleIncrement = 0.04;
	UPROPERTY(EditAnywhere, Category = "Collectible")
	float MassIncrement = 0.015; 
	UPROPERTY(EditAnywhere, Category = "Collectible")
	int MaxNbCollectibles = 80;*/
	float AngleTowardsFloor = -85.f;
	float testCam2 = 0.f;
	UPROPERTY(EditAnywhere, Category = "Collectible")
	UNiagaraSystem* CollectCollectibleVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collectible")
	int NbCollectibles = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	float ItemTimeLeft = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Items")
	float ItemTimer = 5.f;
	UPROPERTY(BlueprintReadWrite, Category = "Items")
	bool IsPlaying = false;
	UFUNCTION(BlueprintCallable)
	void CollectCollectible(FVector CollectibleLocation);

	UFUNCTION(BlueprintCallable)
	void SetPlayerItem(int ItemIndex);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items");
	TEnumAsByte<Items> CurrentItem = Items::Boost;
	
	// Getters
	UFUNCTION(BlueprintCallable)
	float GetXValue();
	UFUNCTION(BlueprintCallable)
	float GetYValue();
	float GetCurrentMass();
	float GetVerticalAngle();
	float GetHorizontalAngle();
	
	int GetCollectiblesCount();

	FVector GetCameraForwardVector();
	FVector GetCameraUpVector();
	FVector GetCameraRightVector();

	UFUNCTION(BlueprintCallable)
	void RemoveAllPlayerMoveSpeed();
	void DisableInputs();
	bool LinearMovementIsDisabled = false;

	AActor* Skydome;
	UPROPERTY(BlueprintReadWrite)
	bool SkydomeIsDisabled = false;
};