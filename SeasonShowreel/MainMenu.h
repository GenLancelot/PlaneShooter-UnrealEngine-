// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameConfiguration.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class SEASONSHOWREEL_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	/**
	* Sets default values
	*/
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	/**
	* Function to bind actions to buttons
	*/
	virtual void NativeConstruct() override;

	/**
	* Function starts new game
	*/
	UFUNCTION()
	void StartNewGame();

	/**
	* Function starts last saved game
	*/
	UFUNCTION()
	void ContinueLastGame();

	/**
	* Function to get playername from input box
	* 
	* @param InText - inputed text
	* @param InCommitType - how input box was commited
	*/
	UFUNCTION()
	void UpdatePlayerName(const FText& InText, ETextCommit::Type InCommitType);

	/**
	* Function to show leaderboard
	*/
	UFUNCTION()
	void LaunchLeaderboard();

	/**
	* Function to quit from game
	*/
	UFUNCTION()
	void QuitGame();

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LeaderboardButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PlayerNameBox;
};
