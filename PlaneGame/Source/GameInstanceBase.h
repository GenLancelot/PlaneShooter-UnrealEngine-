// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveCurrentGame.h"
#include "SaveLeaderboard.h"
#include "GameConfiguration.h"
#include "GameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class SEASONSHOWREEL_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY()
	USaveCurrentGame* SaveGameObject;

	UPROPERTY()
	USaveLeaderboard* SaveLeaderboardObject;

	/**
	* Function loads data from SaveGameObject
	*/
	UFUNCTION()
	void LoadGame();

	/**
	* Function saves data to SaveGameObject
	*/
	UFUNCTION()
	void SaveGame();

	/**
	* Function loads data from SaveLeaderboardObject
	*/
	UFUNCTION()
	void LoadLeaderboard();

	/**
	* Function saves data to SaveLeaderboardObject
	*/
	UFUNCTION()
	void SaveLeaderboard();

	/**
	* Function to get player name
	*/
	FString GetPlayerName() { return PlayerName; }

	/**
	* Function to update player name
	*
	* @param NewName - name provided in widget
	*/
	void SetPlayerName(FString NewName) { PlayerName = NewName; }

	/**
	* Function to check if saved game should be loaded
	*
	*/
	bool ShouldLoadSavedGame() { return bShouldLoadSavedGame; }

	/**
	* Function to set if saved game should be loaded
	*
	* @param bShouldLoadSavedGame - if game should be loaded
	*/
	void SetShouldLoadSavedGame(bool bShould) { bShouldLoadSavedGame = bShould; }


	int32 GetCurrentLevel() { return CurrentLevel; }

	void SetCurrentLevel(int32 NewLevel) { CurrentLevel = NewLevel; UpdateLevelName(); }

	void NextLevel() { ++CurrentLevel; UpdateLevelName(); }

	void PrevLevel() { --CurrentLevel; UpdateLevelName(); }

	void UpdateLevelName() { CurrentLevelName = "Level" + FString::FromInt(CurrentLevel); }

	FName GetCurrentLevelName() { return FName(*CurrentLevelName); }

	bool GetNewLevel() { return bNewLevel; }

	void SetNewLevel(bool NewLevel) { bNewLevel = NewLevel; }

private:

	FString PlayerName = "Unnamed Player";

	int32 CurrentLevel = 1;

	FString CurrentLevelName = "Level" + FString::FromInt(CurrentLevel);

	bool bShouldLoadSavedGame = false;

	bool bNewLevel = false;

	/**
	* Function to log some information about save gmae
	*
	* @param IsSaved - if game was saved with no error
	*/
	void Logging(const bool IsSaved);

};
