// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Gun.h"

AMannequin::AMannequin()
{
	PrimaryActorTick.bCanEverTick = false;

	// Create a CameraComponent
	FPCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FP_Camera"));
	FPCameraComponent->SetupAttachment(GetCapsuleComponent());
	FPCameraComponent->RelativeLocation = FVector(4.5f, 1.75f, 64.f); // Position the camera
	FPCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	MeshFP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Arms"));
	MeshFP->SetupAttachment(FPCameraComponent);
	MeshFP->SetOnlyOwnerSee(true);
	MeshFP->bCastDynamicShadow = false;
	MeshFP->CastShadow = false;
	MeshFP->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	MeshFP->RelativeLocation = FVector(-12.0f, -5.0f, -160.0f);
}

void AMannequin::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	SpawnGun();
}

void AMannequin::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent)
		return;

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
}

void AMannequin::SpawnGun()
{
	// try and spawn gun
	if (!GunClass)
		return;

	if (!MeshFP->GetSocketByName(FName("GripPoint")))
		return;

	//Set Spawn Collision Handling Override
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// spawn the projectile at the muzzle
	EquippedGun = GetWorld()->SpawnActor<AGun>(
		GunClass,
		MeshFP->GetSocketLocation(FName("GripPoint")),
		MeshFP->GetSocketRotation(FName("GripPoint")),
		ActorSpawnParams
		);

	//Attachment is done here because the skelton is not yet created in the constructor
	EquippedGun->AttachToComponent(MeshFP, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AMannequin::PullTrigger()
{
	if (!EquippedGun)
		return;

	EquippedGun->Fire();

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = MeshFP->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

float AMannequin::TakeDamage(float DamageAmount, class AController * EventInstigator, AActor * DamageCauser)
{
	int32 DamagePoints = FMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);
	if (DamagePoints < 1)
		return 0;

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		Die();
	}

	return DamageToApply;
}

void AMannequin::Die()
{
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
