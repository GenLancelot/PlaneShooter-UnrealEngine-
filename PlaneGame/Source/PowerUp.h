// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameConfiguration.h"
#include "PowerUp.generated.h"

UENUM(BlueprintType)
enum class EPowerUpType : uint8 {
	Bullet,
	SpeedUp,
	Bomb,
	Points,
	ExtraLife
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPowerUpTakenEvent, EPowerUpType, PowerUpType);

UCLASS()
class SEASONSHOWREEL_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	APowerUp();

	/**
	* Function to act on powerup take event
	*/
	void Take();
	/**
	* Function to load data from savegame
	*/
	void LoadedData(bool bTaken) { if (bTaken) Destroy(); }

	UPROPERTY(EditAnywhere)
		EPowerUpType PowerUpType;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
		FPowerUpTakenEvent PowerUpTakenEvent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	
	UPROPERTY(EditAnywhere)
		class USceneComponent* RootScene;

	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;
};
