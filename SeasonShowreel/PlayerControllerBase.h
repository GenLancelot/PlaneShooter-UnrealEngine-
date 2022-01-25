// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameConfiguration.h"
#include "PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SEASONSHOWREEL_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
public:

	/**
	* Function to enable or disable control
	* 
	* bSetPlayerEnabled - if player can move
	*/
	void SetPlayerEnabledState(bool bSetPlayerEnabled);
};
