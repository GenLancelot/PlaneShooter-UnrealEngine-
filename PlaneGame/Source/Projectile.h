// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameConfiguration.h"
#include "Projectile.generated.h"

UCLASS()
class SEASONSHOWREEL_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* ParticleTrail;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
		TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = "Damage", meta = (AllowPrivateAccess = "true"))
		float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
		float MovementSpeed = 1300.f;

	UPROPERTY(EditAnywhere, Category = "Effects")
		UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
		USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
		USoundBase* LaunchSound;

	/**
	* Function called when projectile hit other component
	* 
	* @param HitComp - current actor component which detected hit
	* @param OtherActor - hitted actor
	* @param OtherComp - hitted component of other actor
	* @param NormalImpulse - vector of hit impulse
	* @outparam Hit - out variable which containts hit data
	*/
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
