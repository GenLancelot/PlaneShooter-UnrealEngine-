// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "GameConfiguration.h"
#include "ShowreelGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class SEASONSHOWREEL_API AShowreelGameHUD : public AHUD
{
	GENERATED_BODY()

public:

	AShowreelGameHUD();

	/**
	* Function to draw HUD
	*/
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "MainMenu")
		TSubclassOf<class UUserWidget> MainMenuWidgetClass;

private:

	 class UMainMenu* MainMenuWidget;
	 
};
