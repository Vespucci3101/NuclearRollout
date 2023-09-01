// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball_Pawn.h"
#include "AttackComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "BallCapsuleComponent.h"
#include "BallGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraFunctionLibrary.h"
// Sets default values
ABall_Pawn::ABall_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetUseCCD(true);
	RootComponent = StaticMesh;

	CapsuleComponent = CreateDefaultSubobject<UBallCapsuleComponent>(TEXT("Ball Capsule"));
	CapsuleComponent->SetupAttachment(StaticMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(StaticMesh);

	SpringArmHolder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spring Arm Holder"));
	SpringArmHolder->SetupAttachment(SpringArm);

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Spring Arm"));
	CameraSpringArm->SetupAttachment(SpringArmHolder);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);

	StaticMesh->SetLinearDamping(1.0);
	StaticMesh->SetAngularDamping(2.0);
}

// Called when the game starts or when spawned
void ABall_Pawn::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->SetMassOverrideInKg(NAME_None, CurrentMass); 
	ItemTimeLeft = ItemTimer;
	GameInstance = Cast<UBallGameInstance>(UGameplayStatics::GetGameInstance(this));

	FVector SpawnVector = GetActorLocation();
	FVector SpringForwardVector = SpringArm->GetForwardVector();
	SpringForwardVector.Z = 0;
	SpawnVector += SpringForwardVector * ItemSpawnDistance * RootComponent->GetRelativeScale3D();
	if (GetWorld()->GetName() != "alpha_0") ItemHologram = GetWorld()->SpawnActor<AActor>(ItemHologramClass, FVector(0, 0, 0), FRotator(0, 0, 0));
	
	LinearMovementIsDisabled = false;
	IsPlaying = false;
	
	if (!GameInstance->IsAutoCamera)
	{
		SpringArm->bInheritPitch = SpringArm->bInheritRoll = SpringArm->bInheritYaw = false;
		SpringArm->bEnableCameraRotationLag = false;
		SpringArm->SetUsingAbsoluteRotation(true);
	}
	else if (GameInstance->IsAutoCamera)
	{
		SpringArm->bInheritPitch = SpringArm->bInheritRoll = SpringArm->bInheritYaw = true;
		SpringArm->bEnableCameraRotationLag = true;
		SpringArm->CameraRotationLagSpeed = 1.f;
		SpringArm->SetUsingAbsoluteRotation(false);
	}
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Skydome"), OutActors);
	for (AActor* ActorToAttach : OutActors)
	{
		// ActorToAttach->AttachToComponent(CameraSpringArm, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Skydome = ActorToAttach;
	}
	SpringArm->AddWorldRotation(FRotator(0.f, 0.f, -8.f));
	SkydomeIsDisabled = false;
}

FVector ABall_Pawn::GetForceVector(float Value, FVector RefVector)
{
	if (GetWorld()->GetTimeSeconds() < 2.f) return FVector::ZeroVector;
	bool HitResult = false;
	AActor* HitActor = CapsuleComponent->GroundCheckHit(HitResult);
	const float DotProductModifier = FMath::Sign(DotProductResult()) < 0 ? 0 : DotProductResult();
	// Can go faster than normal in a slope
	const float CurrentMaxVelocity = FMath::Sign(DotProductResult()) < 0 ? MaxVelocity : MaxVelocity + (MaxVelocity * DotProductResult());
	if (CurrentVelocity >= CurrentMaxVelocity) return FVector::ZeroVector;
	float CurrentMoveSpeed = HitResult ? MoveSpeed : MoveSpeed / 2;
	// Change this for slope downward force
	// float MoveSpeedModifier =  MoveSpeed + MoveSpeed * DotProductModifier;
	float MoveSpeedModifier =  CurrentMoveSpeed + CurrentMoveSpeed * DotProductModifier;
	if(RootComponent->GetRelativeScale3D().X > 1.0)
	{
		MoveSpeedModifier -= ((RootComponent->GetRelativeScale3D().X - 1.f) * 400.f);
	}
	else
	{
		MoveSpeedModifier += (1.f - RootComponent->GetRelativeScale3D().X) * 4000.f;
	}
	FVector ForceVector = RefVector;
	ForceVector.X = (ForceVector.X * Value * MoveSpeedModifier);
	ForceVector.Y = (ForceVector.Y * Value * MoveSpeedModifier);
	if (!HitResult) ForceVector += FVector(0.f, 0.f, -3000.f);
	else if (InSlope) ForceVector += FVector(0.f, 0.f, -1000.f);
	return ForceVector;
}

void ABall_Pawn::Move(float Value)
{
	if (LinearMovementIsDisabled || GameInstance->IsGamePaused) return;
	if (Value >= 0.5) {
		StaticMesh->SetLinearDamping(0.5);
	}
	else {
		StaticMesh->SetLinearDamping(1.0);
	}
	YValue = -Value;
	StaticMesh->AddForce(GetForceVector(Value, Camera->GetForwardVector()), NAME_None, true);

}

void ABall_Pawn::Turn(float Value)
{
	if (LinearMovementIsDisabled || GameInstance->IsGamePaused) return;
	XValue = -Value;
	StaticMesh->AddForce(GetForceVector(Value, Camera->GetRightVector()) , NAME_None, true);
}

void ABall_Pawn::TurnCamera(float Value)
{
	if (LinearMovementIsDisabled || GameInstance->IsGamePaused) return;
	if (Value != 0.f)
	{
		float RotationSpeed = Value * GetWorld()->GetDeltaSeconds() * 130.f;
		SpringArm->AddWorldRotation(FRotator(0.f, RotationSpeed, 0.f));
		// SpringArm->SetWorldRotation(FRotator(SpringArm->GetComponentRotation().Pitch,SpringArm->GetComponentRotation().Yaw, SpringArm->GetComponentRotation().Roll + (Value * 2.f)));
	}
}


void ABall_Pawn::RaiseCamera(float Value)
{
	if (LinearMovementIsDisabled || GameInstance->IsGamePaused) return;
	if (Value != 0.f && !GameInstance->IsAutoCamera)
	{
		float RotationSpeed = Value * GetWorld()->GetDeltaSeconds() * 130.f;
		ManualCamAngle -= RotationSpeed;
		ManualCamAngle = FMath::Clamp(ManualCamAngle, -20.f, 6.f);
	}
}



// Called every frame
void ABall_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!GameInstance->IsGamePaused) CurrentVelocity = StaticMesh->GetComponentVelocity().Length() / 100000 * 3600;
	// SpringArm->SetWorldRotation(FRotator(-90.f, SpringArm->GetComponentRotation().Yaw,SpringArm->GetComponentRotation().Roll));
	// Skydome->SetActorLocation(CameraSpringArm->GetComponentLocation());
	// Skydome->SetActorRotation(FRotator(CameraSpringArm->GetComponentRotation().Pitch + 15.f, CameraSpringArm->GetComponentRotation().Yaw, CameraSpringArm->GetComponentRotation().Roll));
	// Skydome->AddActorLocalRotation(FRotator(0.f,-Skydome->GetActorRotation().Yaw, 0.f));
	RotateCameraSpringArm(DeltaTime);
	if (GameInstance->IsAutoCamera) RotateSpringArmToFollowSpeed(DeltaTime);
	
	if (!SkydomeIsDisabled)
	{
		Skydome->SetActorLocation(CameraSpringArm->GetComponentLocation());
		Skydome->SetActorRotation(FRotator(CameraSpringArm->GetComponentRotation().Pitch + 15.f, CameraSpringArm->GetComponentRotation().Yaw, CameraSpringArm->GetComponentRotation().Roll));
		Skydome->AddActorLocalRotation(FRotator(0.f, -Skydome->GetActorRotation().Yaw, 0.f));
	}
	

	if (!ItemHologram) return;

	SetItemHologramMesh();

	// Items timer
	if (ItemUsed)
	{
		ItemTimeLeft -= DeltaTime;

		// We get the item that we spawned
		TArray<AActor*> boostPads;
		TArray<AActor*> springPads;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), BoostPadClass, boostPads);
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpringClass, springPads);
	}

	if (ItemTimeLeft <= 0) {
		ItemUsed = false;
		ItemTimeLeft = ItemTimer;
	}

}
// Called to bind functionality to input
void ABall_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("JoystickY", this, &ABall_Pawn::Move);
	PlayerInputComponent->BindAxis("JoystickX", this, &ABall_Pawn::Turn);
	PlayerInputComponent->BindAxis("TurnCamera", this, &ABall_Pawn::TurnCamera);
	PlayerInputComponent->BindAxis("RaiseCamera", this, &ABall_Pawn::RaiseCamera);
	PlayerInputComponent->BindAction(TEXT("UseItem"), IE_Pressed, this, &ABall_Pawn::UseItem);
	PlayerInputComponent->BindAction(TEXT("ToggleCamera"), IE_Pressed, this, &ABall_Pawn::ToggleCamera);
}

void ABall_Pawn::CollectCollectible(FVector CollectibleLocation)
{
	// Increment the number of Collectibles.
	NbCollectibles++;

	if (CollectCollectibleVFX == nullptr) return;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CollectCollectibleVFX, CollectibleLocation);

	// Update SphereMesh.
	//AdjustMeshProperties(true);
}

float ABall_Pawn::GetCurrentMass()
{
	return CurrentMass;
}

int ABall_Pawn::GetCollectiblesCount()
{
	return NbCollectibles;
}


float ABall_Pawn::CalculateCameraSpringArmPitch()
{
	if (DotProductCamera() <= 0)
	{
		return CameraSpringArmPitch;
	}
	else {
		return CameraSpringArmPitch + (FMath::RadiansToDegrees(FMath::Acos(DotProductCamera()) / 4.f));
	}
	return CameraSpringArmPitch;
}

void ABall_Pawn::RotateSpringArmToFollowSpeed(float DeltaTime)
{
	const FRotator RotateTowardsGround = FRotator(AngleTowardsFloor, 0.f, 0.f);
	SpringArm->SetWorldRotation(FRotator(RotateTowardsGround.Pitch, SpringArm->GetComponentRotation().Yaw, SpringArm->GetComponentRotation().Roll));
	const FRotator RotateTowardsSpeed = FRotationMatrix::MakeFromX(GetVelocity().GetSafeNormal2D()).Rotator();
	SpringArm->SetWorldRotation(FRotator(SpringArm->GetComponentRotation().Pitch, RotateTowardsSpeed.Yaw, RotateTowardsSpeed.Roll));

}

void ABall_Pawn::RotateCameraSpringArm(float DeltaTime)
{
	// Slow down when not moving
	if (XValue == 0.0f && YValue == 0.0f && GetVelocity().Length() > 0.0f)
	{
		StaticMesh->ComponentVelocity = FMath::Lerp(GetVelocity(), (0.f, 0.f), MoveSpeed * 0.4f * DeltaTime);
	}
	// Going forward tilt the stage more than going backwards
	
	VerticalRotationSpringArm = FMath::Sign(YValue) < 0 ? YValue - 15 - (YValue * MaxVerticalAngleForward) : YValue - 15 - (YValue * MaxVerticalAngleBackward);
	VerticalRotationSpringArm += ManualCamAngle;
	
	DotProductCamera();
	if (InSlope) VerticalRotationSpringArm -= 15.f;
	
	HorizontalRotation = XValue * MaxHorizontalAngle;
	// This works fine, the last parameter is the speed at which the camera tilts
	CameraSpringArm->SetWorldRotation(FMath::RInterpTo(CameraSpringArm->GetComponentRotation(),
								 FRotator(VerticalRotationSpringArm, CameraSpringArm->GetComponentRotation().Yaw, HorizontalRotation),
									  DeltaTime, 3.f));
}

void ABall_Pawn::SetItemHologramMesh()
{
	if (ItemHologram == nullptr) return;
	if (ItemUsed || !IsPlaying)
	{
		ItemHologram->SetActorHiddenInGame(true);
		return;
	}

	TArray<UStaticMeshComponent*> StaticMeshComps;
	ItemHologram->GetComponents<UStaticMeshComponent*>(StaticMeshComps);

	if (CurrentItem == Items::Spring) StaticMeshComps[0]->SetStaticMesh(SpringMesh);
	else StaticMeshComps[0]->SetStaticMesh(BoostPadMesh);
	
	TArray<FVector> impactPoints = GetItemRaycastImpactPoints();

	if (impactPoints.Num() <= 0) {
		ItemHologram->SetActorHiddenInGame(true);
		return;
	}
		
	FVector Pitch = impactPoints[0] - impactPoints[1];
	FVector Roll = impactPoints[0] - impactPoints[2];

	ItemHologram->SetActorLocation(impactPoints[0]);

	FRotator Rotation = CameraSpringArm->GetForwardVector().Rotation();

	Rotation.Pitch = Pitch.Rotation().Pitch;
	Rotation.Roll = Roll.Rotation().Pitch;

	ItemHologram->SetActorRotation(Rotation);
	if (StaticMesh->GetRelativeScale3D().X >= 2.5f) ItemHologram->SetActorRelativeScale3D(StaticMesh->GetRelativeScale3D());
	else ItemHologram->SetActorRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	ItemHologram->SetActorHiddenInGame(false);
}

/**
 * @brief We need this to know the degree of the slope impact 
 * @return The dot product of the Up Vector of the floor and the velocity
 */
float ABall_Pawn::DotProductResult()
{
	bool HitResult = false;
	AActor* HitActor = CapsuleComponent->GroundCheckHit(HitResult);
	if (HitResult && HitActor) {
		return FVector::DotProduct(HitActor->GetActorUpVector(), GetVelocity().GetSafeNormal2D());
	}
	return 0.0f;
}

float ABall_Pawn::DotProductCamera()
{
	bool HitResult = false;
	AActor* HitActor = CapsuleComponent->GroundCheckHit(HitResult);

	if (HitResult && HitActor) {
		FHitResult hitData;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		bool HasHit = GetWorld()->LineTraceSingleByChannel(hitData, StaticMesh->GetComponentLocation(), StaticMesh->GetComponentLocation() + FVector(0, 0, -1) * 500.f, ECC_Visibility, QueryParams);
		if (HasHit && hitData.GetActor()->ActorHasTag("Slope"))
		{
			// DrawDebugLine(GetWorld(), StaticMesh->GetComponentLocation(), CapsuleComponent->GetComponentLocation() + FVector(0, 0, -1) * 500.f, FColor::Red);
			// DrawDebugLine(GetWorld(), StaticMesh->GetComponentLocation(), CapsuleComponent->GetComponentLocation() + hitData.Normal * 500.f, FColor::Green);
			InSlope = true;
			return  FVector::DotProduct(hitData.Normal, Camera->GetForwardVector().GetSafeNormal2D());
		}
		// return FVector::DotProduct(HitActor->GetActorUpVector(), Camera->GetForwardVector().GetSafeNormal2D());
	}
	InSlope = false;
	return 0.0f;
}



void ABall_Pawn::UseItem()
{
	if (!IsPlaying) return;
	if (!BoostPadClass || !SpringClass || ItemUsed || ItemHologram == nullptr)
	{
		NotAbleToPlaceItemEvent();
		return;
	}
	AActor* SpawnedItem;

	// Since we want only one instance of creation per level, we find the other ones and destroy them
	TArray<AActor*> boostPads;
	TArray<AActor*> springPads;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BoostPadClass, boostPads);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SpringClass, springPads);

	for (AActor* item : boostPads) item->Destroy();
	for (AActor* item : springPads) item->Destroy();

	TArray<FVector> impactPoints = GetItemRaycastImpactPoints();

	if (impactPoints.Num() <= 0) return;

	FVector Pitch = impactPoints[0] - impactPoints[1];
	FVector Roll = impactPoints[0] - impactPoints[2];

	FRotator Rotation = CameraSpringArm->GetForwardVector().Rotation();

	switch (CurrentItem)
	{
	case Items::Boost:
		SpawnedItem = GetWorld()->SpawnActor<AActor>(BoostPadClass, impactPoints[0], Rotation);
		if (StaticMesh->GetRelativeScale3D().X >= 2.5f) SpawnedItem->SetActorRelativeScale3D(StaticMesh->GetRelativeScale3D());
		else SpawnedItem->SetActorRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
		ItemUsed = true;
		break;
	case Items::Spring:
		SpawnedItem = GetWorld()->SpawnActor<AActor>(SpringClass, impactPoints[0], Rotation);
		if (StaticMesh->GetRelativeScale3D().X >= 2.5f) SpawnedItem->SetActorRelativeScale3D(StaticMesh->GetRelativeScale3D());
		else SpawnedItem->SetActorRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
		ItemUsed = true;
		break;
	default:
		SpawnedItem = nullptr;
		break;
	}
	if (SpawnedItem != nullptr)
	{
		Rotation.Pitch = Pitch.Rotation().Pitch;
		Rotation.Roll = Roll.Rotation().Pitch;
		SpawnedItem->SetActorRotation(Rotation);
		CreatingItemEvent();
	}
	
}

void ABall_Pawn::ToggleCamera()
{
	// IsAutoCamera = !IsAutoCamera;
	// SpringArm->bInheritPitch = !CameraSpringArm->bInheritPitch;
	// SpringArm->bInheritRoll = !CameraSpringArm->bInheritRoll;
	// SpringArm->bInheritYaw = !CameraSpringArm->bInheritYaw;
	// if (!IsAutoCamera)
	// {
	// 	SpringArm->bEnableCameraRotationLag = false;
	// 	SpringArm->SetUsingAbsoluteRotation(true);
	// }
	// else
	// {
	// 	SpringArm->bEnableCameraRotationLag = true;
	// 	SpringArm->CameraRotationLagSpeed = 1.f;
	// 	SpringArm->SetUsingAbsoluteRotation(false);
	// }
}

void ABall_Pawn::SetPlayerItem(int ItemIndex)
{
	switch (ItemIndex)
	{
	case 0:
		CurrentItem = Items::Boost;
		break;
	case 1:
		CurrentItem = Items::Spring;
		break;
	default:
		CurrentItem = Items::Boost;
		break;
	}
}

TArray<FVector> ABall_Pawn::GetItemRaycastImpactPoints()
{
	FVector SpawnVector = GetActorLocation();
	FVector SpringForwardVector = CameraSpringArm->GetForwardVector();
	SpringForwardVector.Z = 1.f;
	SpawnVector += SpringForwardVector * ItemSpawnDistance * FVector(RootComponent->GetRelativeScale3D().X + ItemScaleDistanceModifier,
																	 RootComponent->GetRelativeScale3D().Y + ItemScaleDistanceModifier,
																	 FMath::Sqrt(RootComponent->GetRelativeScale3D().Z));

	TArray<FVector> outputs;
	FHitResult Hit1;
	GetWorld()->LineTraceSingleByObjectType(
		Hit1,
		SpawnVector,
		SpawnVector - FVector(0, 0, 2000),
		FCollisionObjectQueryParams::AllStaticObjects
	);

	FHitResult Hit2;
	GetWorld()->LineTraceSingleByObjectType(
		Hit2,
		SpawnVector + FVector(CameraSpringArm->GetForwardVector().X * -10, CameraSpringArm->GetForwardVector().Y * -10, 0.f),
		SpawnVector + FVector(CameraSpringArm->GetForwardVector().X * -10, CameraSpringArm->GetForwardVector().Y * -10, 0.f) - FVector(0, 0, 2000),
		FCollisionObjectQueryParams::AllStaticObjects
	);

	FHitResult Hit3;
	GetWorld()->LineTraceSingleByObjectType(
		Hit3,
		SpawnVector + FVector(CameraSpringArm->GetRightVector().X * 10, CameraSpringArm->GetRightVector().Y * 10, 0.f),
		SpawnVector + FVector(CameraSpringArm->GetRightVector().X * 10, CameraSpringArm->GetRightVector().Y * 10, 0.f) - FVector(0, 0, 2000),
		FCollisionObjectQueryParams::AllStaticObjects
	);

	if (Hit1.GetActor() == nullptr) return outputs;

	outputs.Add(Hit1.ImpactPoint);
	outputs.Add(Hit2.ImpactPoint);
	outputs.Add(Hit3.ImpactPoint);

	return outputs;
}


FVector ABall_Pawn::GetCameraForwardVector()
{
	return Camera->GetForwardVector();
}

float ABall_Pawn::GetHorizontalAngle()
{
	return HorizontalRotation;
}

FVector ABall_Pawn::GetCameraRightVector()
{
	return Camera->GetRightVector();
}

float ABall_Pawn::GetVerticalAngle()
{
	return VerticalRotationSpringArm;
}

FVector ABall_Pawn::GetCameraUpVector()
{
	return Camera->GetUpVector();
}

float ABall_Pawn::GetXValue()
{
	return XValue;
}

float ABall_Pawn::GetYValue()
{
	return YValue;
}

void ABall_Pawn::RemoveAllPlayerMoveSpeed()
{
	DisableInputs();
	StaticMesh->SetPhysicsLinearVelocity(FVector());
	//StaticMesh->SetEnableGravity(false);
}

void ABall_Pawn::DisableInputs()
{
	MoveSpeed = 0.f;
	LinearMovementIsDisabled = true;
}