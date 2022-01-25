// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameConfiguration.h"
#include "SaveCurrentGame.generated.h"


USTRUCT()
struct FPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	struct FPlayerInfo() = default;

	FPlayerInfo(FVector PlayerLocation, FRotator PlayerRotation, float Points, float Health, int32 Bullets, bool bHasSpeedUp, bool bHasBomb) :
	PlayerLocation(PlayerLocation), PlayerRotation(PlayerRotation), 
	Points(Points), Health(Health), Bullets(Bullets), 
	bHasSpeedUp(bHasSpeedUp), bHasBomb(bHasBomb) {}

	UPROPERTY()
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY()
	FRotator PlayerRotation = FRotator::ZeroRotator;

	UPROPERTY()
	float Points = 0.f;

	UPROPERTY()
	float Health = 100.f;

	UPROPERTY()
	int32 Bullets = 0;

	UPROPERTY()
	bool bHasSpeedUp = false;

	UPROPERTY()
	bool bHasBomb = false;

};

USTRUCT()
struct FBotInfo
{
	GENERATED_USTRUCT_BODY()

	struct FBotInfo() = default;

	FBotInfo(FName BotName, bool bDead, float Health) : BotName(BotName), bDead(bDead), Health(Health) {}
	
	UPROPERTY()
	FName BotName = FName("");

	UPROPERTY()
	bool bDead = false;

	UPROPERTY()
	float Health = 100.f;
};

USTRUCT()
struct FPowerUpInfo
{
	GENERATED_USTRUCT_BODY()

	struct FPowerUpInfo() = default;

	FPowerUpInfo(FName PowerUpName, bool bTaken) : PowerUpName(PowerUpName), bTaken(bTaken) {}

	UPROPERTY()
	FName PowerUpName = FName("");

	UPROPERTY()
	bool bTaken = true;

};

UCLASS()
class SEASONSHOWREEL_API USaveCurrentGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	USaveCurrentGame();

	UPROPERTY()
	FPlayerInfo PlayerInfo;

	UPROPERTY()
	TArray<FBotInfo> BotsInfo;

	UPROPERTY()
	TArray<FPowerUpInfo> PowerUpsInfo;

	UPROPERTY()
	int32 CurrentLevel = 1;
};
