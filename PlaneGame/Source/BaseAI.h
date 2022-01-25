// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "GameConfiguration.h"
#include "BaseAI.generated.h"

/**
 * 
 */


UCLASS()
class SEASONSHOWREEL_API ABaseAI : public ABasePawn
{
	GENERATED_BODY()

public:
	/**
	* To get sign of value
	*/
	template<typename T>
	int32 getSign(T x) {
		return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
	}
	/**
	* To check if actor location is beetwen vectors, one of fvector coordinates must have same sign in all tree fvectors
	*/
	template<typename T>
	bool IsValueBetweenTwoOtherValues(T x, T y, T z)
	{
		if (getSign(x) == getSign(y))
			if(getSign(x) == getSign(z)) return true;
		if (y > z)
		{
			return x > z && x < y;
		}
		else
		{
			return x > y && x < z; 
		}
	}

	virtual void Tick(float DeltaTime) override;

	/**
	 * Function which controlling AI movement
	 */
	void Move();

	/**
	 * Function which controlling AI movement variables when AI ends travelling from one location to another
	 */
	void ResetAndMove();

	/**
	 * Function which controlling AI rotating to opposite direction
	 */
	void Rotate();

	/**
	 * Function which controlling AI rotation variables when AI ends rotation
	 */
	void ResetAndRotate();

	/**
	 * Function sets property of paused game
	 */
	void SetPaused(bool bCanMove) { bPaused = bCanMove; }

	/**
	 * Function loads varaibles to Pawn
	 * 
	 * @param IsDead - if actor was dead in saved game
	 * @param Health - actor health if it wasnt dead
	 */
	void LoadedData(bool IsDead, float Health);

	/**
	 * Function loads varaibles to Death Pawns
	 *
	 * @param IsDead - if actor was dead in saved game
	 */
	void LoadedData(bool IsDead = true);

	virtual void ApplyDamage(float Damage) override;

	virtual void Fire() override;

protected:

	virtual void BeginPlay() override;

private:

	/**
	 * Function checks if AI may hit Player with shoot 
	 */
	bool IsGoodToShoot();

	/**
	 * Function checks if AI can see Player in FOV 30 degree
	 */
	bool IsInAimRadius(FVector Direction);

	/**
	 * Function checks if Player is in range
	 */
	bool IsInRange(FVector PlayerLocation);

	/**
	 * Function checks if AI is reloading
	 */
	bool IsRealoading() { return bReloading; }

	/**
	 * Function sets AI location variables
	 */
	void SetupLocation();

	/**
	 * Function sets AI rotation variables
	 */
	void SetupRotation();


	class APlayerPawn* PlayerPawn;

	class APlayGameMode* GameMode;

	FVector FirstLocation;

	FVector SecondLocation;

	FRotator FirstRotation;

	FRotator SecondRotation;

	float TimeBetweenLocation = 5.f;

	float TimeBetweenRotation = 1.f;

	float MovingTimer = 0.f;

	float RotatingTimer = 0.f;

	float Points = 500.f;

	float ReloadTime = 0.5f;

	bool ToSecondLocation = false;

	bool bMoving = true;

	bool bRotating = false;

	bool bPaused = false;

	bool bReloading = false;
};
