// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstanceBase.h"
#include "Kismet/GameplayStatics.h"

void UGameInstanceBase::LoadGame()
{
	SaveGameObject = Cast<USaveCurrentGame>(UGameplayStatics::LoadGameFromSlot("SaveGameSlot", 0));
	if (!SaveGameObject)
	{
		SaveGameObject = Cast<USaveCurrentGame>(UGameplayStatics::CreateSaveGameObject(USaveCurrentGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SaveGameObject, "SaveGameSlot", 0);
	}
}

void UGameInstanceBase::SaveGame()
{
	UGameplayStatics::SaveGameToSlot(SaveGameObject, "SaveGameSlot", 0);
	
}

void UGameInstanceBase::LoadLeaderboard()
{

	SaveLeaderboardObject = Cast<USaveLeaderboard>(UGameplayStatics::LoadGameFromSlot("Leaderboard", 0));
	if (!SaveLeaderboardObject)
	{
		SaveGameObject = Cast<USaveCurrentGame>(UGameplayStatics::CreateSaveGameObject(USaveCurrentGame::StaticClass()));
		UGameplayStatics::SaveGameToSlot(SaveLeaderboardObject, "Leaderboard", 0);
	}
	
}

void UGameInstanceBase::SaveLeaderboard()
{
	UGameplayStatics::SaveGameToSlot(SaveLeaderboardObject, "Leaderboard", 0);
}

void UGameInstanceBase::Logging(const bool IsSaved)
{
	if (IsSaved)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Game saved."));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Cannot save the game."));
	}
}