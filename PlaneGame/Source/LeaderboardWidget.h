// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameConfiguration.h"
#include "LeaderboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEASONSHOWREEL_API ULeaderboardWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/**
	* Sets default values
	*/
	ULeaderboardWidget(const FObjectInitializer& ObjectInitializer);

	/**
	* Function to bind actions to buttons
	*/
	virtual void NativeConstruct() override;

	/**
	* Function to get leaderboard from saved instance
	*/
	void GetLeaderboardFromFile();

	/**
	* Function to set leaderboard to text blocks
	* 
	* @param TableRank - string containing all posiotion in leaderboard
	* @param TableName - string containing all names in leaderboard
	* @param TableScore - string containing all points in leaderboard
	*/
	void SetLeaderboardToTextBlock(FString TableRank, FString TableName, FString TableScore);

	/**
	* Function to back to main menu
	*/
	UFUNCTION()
	void ReturnToMainMenu();

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Rank;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Name;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Score;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;

private:

	class UGameInstanceBase* GameInstance;
};
