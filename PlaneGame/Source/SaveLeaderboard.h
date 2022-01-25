// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameConfiguration.h"
#include "SaveLeaderboard.generated.h"



/**
 * 
 */

USTRUCT()
struct FTableRow
{
	GENERATED_USTRUCT_BODY()
	
	struct FTableRow() = default;

	FTableRow(int32 rank, FString name, float points) : rank(rank), name(std::move(name)), points(points) {}

	UPROPERTY()
	int32 rank = 0;

	UPROPERTY()
	FString name = "";

	UPROPERTY()
	float points = 0.f;

	
};

UCLASS()
class SEASONSHOWREEL_API USaveLeaderboard : public USaveGame
{
	GENERATED_BODY()
	
public:

	USaveLeaderboard();

	UPROPERTY()
	TArray<FTableRow> Leaderboard;

};
