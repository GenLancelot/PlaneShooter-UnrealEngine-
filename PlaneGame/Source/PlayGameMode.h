// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SeasonShowreelGameModeBase.h"
#include "GameConfiguration.h"
#include "PlayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SEASONSHOWREEL_API APlayGameMode : public ASeasonShowreelGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "PauseMenu")
		TSubclassOf<class UUserWidget> PauseMenuWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Statistics")
		TSubclassOf<class UUserWidget> StatisticWidgetClass;

	/**
	* Function to get numbers of non-dead AIs 
	*/
	int32 GetTargetsCount() const;

	/**
	* Function to pause game
	* 
	* @param bPaused - if game will be paused
	*/
	void SetGamePaused(bool bPaused);

	/**
	* Function to update health
	* 
	* @param Percent - percent of player health
	*/
	void UpdateHealth(float Percent);

	/**
	* Function to update points
	*
	* @param Points - current player points
	*/
	void UpdatePoints(float Points);

	/**
	* Function to update bullets
	*
	* @param Count - how many ammo player have
	*/
	void UpdateBullets(int32 Count);

	/**
	* Function to update bomb
	*
	* @param bHave - if actor has bomb
	*/
	void UpdateBomb(bool bHave);

	/**
	* Function to update boos
	*
	* @param bHave - if actor has boost
	*/
	void UpdateSpeedUp(bool bHave);

	/**
	* Function to update player name
	*
	* @param NewName - name provided in widget
	*/
	void UpdatePlayerName(FString NewName) { PlayerName = NewName; }

	/**
	* Function to act if any pawn was killed
	*
	* @param bPlayer - if player was killed
	*/
	void PawnKilled(bool bPlayer);

	/**
	* Function to parse data to save game
	*
	*/
	void ParseDataToSave();

	void PlayerReachedEndPlate() { EndLevel(); MoveToNextLevel(); }
protected:

	void BeginPlay() override;

private:

	/**
	* Function called at end game
	*
	* @param bPlayerWin - if player won game
	*/
	void EndGame(bool bPlayerWin);

	void EndLevel();

	void ParsePlayerData();

	void MoveToNextLevel();

	void LoadData();
	/**
	* Function goes to main menu after game end
	*/
	void AfterGameEnd();

	float StartDelay;

	int TargetsCount;

	FString PlayerName = "Unnamed Player";

	class APlayerControllerBase* PlayerController;

	class APlayerPawn* Player;

	class UPauseManu* PauseMenuWidget;

	class UStatisicWidget* StatisticWidget;

	class UGameInstanceBase* GameInstance;
};
