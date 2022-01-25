// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
#include "GameConfiguration.h"
#include "BasePawn.generated.h"

UCLASS()
class SEASONSHOWREEL_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:

	/**
	 * Sets default values for this pawn's properties
	 */
	ABasePawn();

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* DeathParticle;

	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AProjectile> ProjectileType;

	UPROPERTY(EditAnywhere, Category = "Components")
		USceneComponent* ProjectileSpawnPoint;

	FCollisionQueryParams TraceParams;

	/**
	 * Function to spawn projectile
	 */
	virtual void Fire();

	/**
	 * Function to reduce Pawn health
	 * 
	 * @param Damage - damage object which hit actor
	 */
	virtual void ApplyDamage(float Damage);

	/**
	 * Function which is called every frame
	 * 
	 * @param DeltaTime - time elapsed between this and last frame
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * Function to get percentage health of Pawn
	 */
	//float GetHealthPercent() { return CurrentHealth / MaxHealth; };
	float GetHealthPercent();
protected:

	float MaxHealth = 100.f;

	float CurrentHealth = MaxHealth;

	double LocationZ = 0.f;

	/**
	 * Function which is called when actor is spawneds
	 */
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Status", meta = (AllowPrivateAccess = "true"))
		bool bDead = false;
	
	FVector ActorLocation;

	UWorld* World;
};
