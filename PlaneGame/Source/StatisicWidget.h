// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameConfiguration.h"
#include "StatisicWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEASONSHOWREEL_API UStatisicWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UStatisicWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Points;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Bullets;

	UPROPERTY(meta = (BindWidget))
		class UImage* Bomb;

	UPROPERTY(meta = (BindWidget))
		UImage* SpeedUp;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Health;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* EndState;

	/**
	* Function to show end result of game
	*
	* @param bPlayerWin - is player won game
	*/
	void ShowEndState(bool bPlayerWin);

	void UpdateHealth(float Percent);

	void UpdatePoints(float NewPoints);

	void UpdateBullets(int32 Count);

	void UpdateBomb(bool bHave);

	void UpdateSpeedUp(bool bHave);

};
