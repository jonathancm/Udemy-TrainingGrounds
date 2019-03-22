// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Mannequin.generated.h"

UCLASS()
class TESTINGGROUND_API AMannequin : public ACharacter
{
	GENERATED_BODY()

public:	
	AMannequin();
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	float TakeDamage(float DamageAmount, class AController * EventInstigator, AActor * DamageCauser);
	//float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	int32 GetCurrentHealth() { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PullTrigger();

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMeshFP() const { return MeshFP; }

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFPCameraComponent() const { return FPCameraComponent; }

protected:
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class UAnimMontage* FireAnimation;

	virtual void BeginPlay() override;
	void SpawnGun();

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	int32 MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = "Health")
	int32 CurrentHealth = 1;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* MeshFP;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPCameraComponent;

	/** Gun class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AGun> GunClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AGun* EquippedGun = nullptr;

	void Die();
};
