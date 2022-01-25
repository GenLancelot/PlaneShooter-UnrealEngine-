// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameConfiguration.h"
#include "PauseManu.generated.h"

/**
 * 
 */
UCLASS()
class SEASONSHOWREEL_API UPauseManu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/**
	* Sets default values
	*/
	UPauseManu(const FObjectInitializer& ObjectInitializer);

	/**
	* Function to bind actions to buttons
	*/
	virtual void NativeConstruct() override;

	/**
	* Function to resume game
	*/
	UFUNCTION()
		void Resume();

	/**
	* Function to leave game
	*/
	UFUNCTION()
		void Leave();

	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

private:

	class APlayGameMode* GameMode;
};
