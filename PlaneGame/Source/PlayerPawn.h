// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "PlayGameMode.h"
#include "GameConfiguration.h"
#include "PlayerPawn.generated.h"

/**
 * 
 */


UCLASS()
class SEASONSHOWREEL_API APlayerPawn : public ABasePawn
{
	GENERATED_BODY()

public:

	APlayerPawn();

	virtual void Tick(float DeltaTime) override;

	/**
	* Function called to bind functionality to input
	* 
	* @param PlayerInpuComponent - Component which controls input
	*/
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* Function called when actor collect boost powerup
	*/
	UFUNCTION(BlueprintCallable)
	void TakeBoost() { bHaveBoost = true; GameMode->UpdateSpeedUp(bHaveBoost);  }

	/**
	* Function called when actor collect more bullets
	*/
	UFUNCTION(BlueprintCallable)
	void TakeBullet(int32 inCrate) { BulletCount += inCrate; if (BulletCount > MaxBulletsCount) BulletCount = MaxBulletsCount; GameMode->UpdateBullets(BulletCount); }

	/**
	* Function called when actor collect bomb
	*/
	UFUNCTION(BlueprintCallable)
	void TakeBomb() { bHaveBomb = true; GameMode->UpdateBomb(bHaveBomb);}

	/**
	* Function called when actor collect extra points
	*/
	UFUNCTION(BlueprintCallable)
	void TakePoints(float ExtraPoints) { Points += ExtraPoints; GameMode->UpdatePoints(Points); }

	/**
	* Function called when actor collect extra life
	*/
	UFUNCTION(BlueprintCallable)
		void TakeExtraLife(float ExtraLife) {  CurrentHealth += ExtraLife; if (CurrentHealth > MaxHealth) CurrentHealth = MaxHealth; GameMode->UpdateHealth(CurrentHealth); }

	/**
	* Function to get current points of actor
	*/
	float GetPoints() { return Points;};

	/**
	* Function to set current state of game
	*/
	void SetGamePaused(bool ChangeState) { bPaused = ChangeState; }

	/**
	 * Function to reduce Pawn health
	 *
	 * @param Damage - damage object which hit actor
	 */
	virtual void ApplyDamage(float Damage) override;

	/**
	 * Function to set data to saved game
	 *
	 * @param LastPlayerLocation - last player location to save
	 * @param LastPlayerRotation - last player rotation to save
	 * @param bHasBom - if actor has bomb 
	 * @param bHasSpeedUp - if actor has boost 
	 * @param hasBullets - number of bullets to save
	 * @param hasPoints - points to save
	 * @param hasHealth - actor health to save
	 */
	void SetLoadedData(FVector LastPlayerLocation, FRotator LastPlayerRotation, bool bHasBomb, bool bHasSpeedUp, int hasBullets, float hasPoints, float hasHealth);

	void SetLoadedProperties(bool bHasBomb, bool bHasSpeedUp, int hasBullets, float hasPoints, float hasHealth);

	/**
	 * Function to load data from saved game
	 *
	 * @param LastPlayerLocation - last player location in saved game
	 * @param LastPlayerRotation - last player rotation in saved game
	 * @param bHasBom - if actor has bomb in saved game
	 * @param bHasSpeedUp - if actor has boost in saved game
	 * @param hasBullets - number of bullets which actor has in saved game
	 * @param hasPoints - points which actor has in saved game
	 * @param hasHealth - health which actor has in saved game
	 */
	void GetPlayerInfo(FVector& LastPlayerLocation, FRotator& LastPlayerRotation, bool& bHasBomb, bool& bHasSpeedUp, int& hasBullets, float& hasPoints, float& hasHealth);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float MoveSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float RotationSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float Boost = 300.f;

	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		bool bHaveBoost = false;

	UPROPERTY(VisibleAnywhere)
		float Points = 0.f;

	UPROPERTY(EditAnywhere, Category = "Ammunition", meta = (AllowPrivateAccess = "true"))
		int32 BulletCount = 0;

	UPROPERTY(EditAnywhere, Category = "Ammunition", meta = (AllowPrivateAccess = "true"))
		bool bHaveBomb = false;

	UPROPERTY(EditAnywhere, Category = "PowerUpTime")
		float PowerUpTime = 2.f;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AProjectile> BombType;

	/**
	* Function checks if actor has ammo
	*/
	void FireIfCan();

	/**
	* Function controls actor rotation
	*/
	void Rotate();

	/**
	* Function controls actor movement
	*/
	void Move();

	/**
	 * Function to spawn projectile
	 */
	void Fire();

	void DetachBomb();

	/**
	 * Function to pause game
	 */
	void Pause();

	/**
	 * Function to drop bomb
	 */
	void DropBomb();

	/**
	 * Function to apply and control boost
	 */
	void ApplyBoost();

	/**
	 * Function to send variables to widget
	 */
	void UpdateWidget();

	/**
	 * Function to calculate provided input
	 * 
	 * @param Value - provided value 
	 */
	void CalculateMoveInput(float Value);

	/**
	 * Function to calculate provided input
	 *
	 * @param Value - provided value 
	 */
	void CalculateRotateInput(float Value);

	/**
	* Function controls overlap begin events
	* @param OverlappedComp - Current actor overllaped component
	* @param OtherActor - Actor which owns other overlapped component
	* @param OtherComp - other overlapped component
	*/
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	const int32 MaxBulletsCount = 50;

	bool bPaused = false;

	FVector MovementDirection = FVector(0.f, 0.f, 0.f);

	FQuat RotationDirection = FQuat(0.f, 0.f, 0.f, 0.f);

	UPROPERTY()
		class UCameraComponent* Camera;

	UPROPERTY()
		class USpringArmComponent* SpringArm;

	APlayGameMode* GameMode;

	class APlayerControllerBase* PlayerController;

	class AShowreelGameHUD* HUD;

};
