// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameMode.h"
#include "PlayerPawn.h"
#include "PlayerControllerBase.h"
#include "BaseAI.h"
#include "Kismet/GameplayStatics.h"
#include "PauseManu.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "StatisicWidget.h"
#include "SaveLeaderboard.h"
#include "GameInstanceBase.h"
#include "PowerUp.h"


void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
	GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));
	PlayerName = GameInstance->GetPlayerName();
	if (StatisticWidgetClass)
	{
		StatisticWidget = CreateWidget<UStatisicWidget>(GetWorld(), StatisticWidgetClass);

		if (StatisticWidget)
		{
			StatisticWidget->AddToViewport();
		}
	}
	if (GameInstance->GetNewLevel())
	{
		GameInstance->LoadGame();
		if(Player != nullptr)
		Player->SetLoadedProperties(
			SAVEGAME->PlayerInfo.bHasBomb,
			SAVEGAME->PlayerInfo.bHasSpeedUp,
			SAVEGAME->PlayerInfo.Bullets,
			SAVEGAME->PlayerInfo.Points,
			SAVEGAME->PlayerInfo.Health);
		else
			UE_LOG(LogTemp, Warning, TEXT("Loading player properties - doesnt work"));
		GameInstance->SetNewLevel(false);
	}
	if (GameInstance->ShouldLoadSavedGame())
	{
		LoadData();
		GameInstance->SetShouldLoadSavedGame(false);
	}
}

void APlayGameMode::ParseDataToSave()
{
	GameInstance->LoadGame();
	UE_LOG(LogTemp, Warning, TEXT("COJESTGRANE! "));
	Player->GetPlayerInfo(SAVEGAME->PlayerInfo.PlayerLocation, SAVEGAME->PlayerInfo.PlayerRotation,
		SAVEGAME->PlayerInfo.bHasBomb, SAVEGAME->PlayerInfo.bHasSpeedUp, SAVEGAME->PlayerInfo.Bullets,
		SAVEGAME->PlayerInfo.Points, SAVEGAME->PlayerInfo.Health);
	SAVEGAME->BotsInfo.Empty();
	TArray<AActor*> AIs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseAI::StaticClass(), AIs);
	for (AActor* Actor : AIs)
	{
		ABaseAI* Bot = Cast<ABaseAI>(Actor);
		FName BotName = Bot->GetFName();
		SAVEGAME->BotsInfo.Emplace(BotName, false, Bot->GetHealthPercent());
	}
	SAVEGAME->PowerUpsInfo.Empty();
	TArray<AActor*> PowerUps;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerUp::StaticClass(), PowerUps);
	
	for (AActor* Actor : PowerUps)
	{
		FName PowerUpName = Actor->GetFName();
		SAVEGAME->PowerUpsInfo.Emplace(PowerUpName, false);
	}
	SAVEGAME->CurrentLevel = GameInstance->GetCurrentLevel();
	GameInstance->SaveGame();
}


void APlayGameMode::ParsePlayerData()
{
	GameInstance->LoadGame();
	Player->GetPlayerInfo(SAVEGAME->PlayerInfo.PlayerLocation, SAVEGAME->PlayerInfo.PlayerRotation,
		SAVEGAME->PlayerInfo.bHasBomb, SAVEGAME->PlayerInfo.bHasSpeedUp, SAVEGAME->PlayerInfo.Bullets,
		SAVEGAME->PlayerInfo.Points, SAVEGAME->PlayerInfo.Health);
	GameInstance->SaveGame();
}

int32 APlayGameMode::GetTargetsCount() const
{
	TArray<AActor*> AIs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseAI::StaticClass(), AIs);
	return AIs.Num();
}

void APlayGameMode::SetGamePaused(bool bPaused)
{
	if (bPaused && UGameplayStatics::GetCurrentLevelName(GetWorld()) != FString("MainMenuLevel"))
	{
		
		if (PauseMenuWidgetClass)
		{
			PauseMenuWidget = CreateWidget<UPauseManu>(GetWorld(), PauseMenuWidgetClass);
			
			if (PauseMenuWidget)
			{
				
				TArray<AActor*> AIs;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseAI::StaticClass() , AIs);
				for (AActor* Current : AIs)
				{
					ABaseAI* AI = (ABaseAI*)Current;
					AI->SetPaused(true);
				}
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, PauseMenuWidget);
				PauseMenuWidget->AddToViewport();
				PlayerController->SetShowMouseCursor(true);
			}
		}
		
	}
	else
	{
		if (PauseMenuWidget)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			PlayerController->SetShowMouseCursor(false);
			PauseMenuWidget->RemoveFromViewport();
			TArray<AActor*> AIs;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseAI::StaticClass(), AIs);
			for (AActor* Current : AIs)
			{
				ABaseAI* AI = (ABaseAI*)Current;
				AI->SetPaused(false);
			}
			Player->SetGamePaused(false);
		}

	}
}

void APlayGameMode::UpdateHealth(float Percent)
{
	if (StatisticWidget)
	{
		StatisticWidget->UpdateHealth(Percent);
	}
}

void APlayGameMode::UpdatePoints(float Points)
{
	if (StatisticWidget)
	{
		StatisticWidget->UpdatePoints(Points);
	}
}

void APlayGameMode::UpdateBullets(int32 Count)
{
	if (StatisticWidget)
	{
		StatisticWidget->UpdateBullets(Count);
	}
}

void APlayGameMode::UpdateBomb(bool bHave)
{
	if (StatisticWidget)
	{
		StatisticWidget->UpdateBomb(bHave);
	}
}

void APlayGameMode::UpdateSpeedUp(bool bHave)
{
	if (StatisticWidget)
	{
		StatisticWidget->UpdateSpeedUp(bHave);
	}
}

void APlayGameMode::PawnKilled(bool bPlayer)
{
	if (bPlayer)
		EndGame(false);

	if (GetTargetsCount() == 0)
	{
		EndLevel();
	}

}

void APlayGameMode::EndLevel() 
{ 
	if (GameInstance->GetCurrentLevel() == MAXLEVEL) 
		EndGame(true); 
	ParsePlayerData();
}

void APlayGameMode::EndGame(bool bPlayerWin)
{
		
		GameInstance->LoadLeaderboard();
		if (LEADERBOARD->IsValidLowLevel())
		{
			if (!LEADERBOARD->Leaderboard.IsEmpty())
			{
				bool bInList = false;
				for (int32 i = 0; i < LEADERBOARD->Leaderboard.Num(); i++)
				{
					if (Player->GetPoints() > LEADERBOARD->Leaderboard[i].points)
					{
						bInList = true;
						LEADERBOARD->Leaderboard.EmplaceAt(i, i + 1, PlayerName, Player->GetPoints());
						if (LEADERBOARD->Leaderboard.Num() > LEADEARBOARDSIZE)
							LEADERBOARD->Leaderboard.Pop();
						break;
					}
				}
				if (bInList)
				{
					for (int32 i = 0; i < LEADERBOARD->Leaderboard.Num(); i++)
					{
						LEADERBOARD->Leaderboard[i].rank = i + 1;
					}
					GameInstance->SaveLeaderboard();
				}
			}
			else
			{
				LEADERBOARD->Leaderboard.Emplace(1, PlayerName, Player->GetPoints());
				GameInstance->SaveLeaderboard();
			}
		}
		if (StatisticWidget)
		{
			StatisticWidget->ShowEndState(bPlayerWin);
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &APlayGameMode::AfterGameEnd, 3.f);
		}
}

void APlayGameMode::AfterGameEnd()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
}

void APlayGameMode::MoveToNextLevel()
{
	//#TODO:Move to next level - probably will be needed to put playerdata in gameinstance or sth
	UE_LOG(LogTemp, Warning, TEXT("Moving to next level"));
	GameInstance->NextLevel();
	GameInstance->SetNewLevel(true);
	UGameplayStatics::OpenLevel(GetWorld(), GameInstance->GetCurrentLevelName());
}

void APlayGameMode::LoadData()
{
	GameInstance->LoadGame();
	if (GameInstance->GetCurrentLevel() != SAVEGAME->CurrentLevel)
	{
		GameInstance->SetCurrentLevel(SAVEGAME->CurrentLevel);
		UGameplayStatics::OpenLevel(GetWorld(), GameInstance->GetCurrentLevelName());
	}
	Player->SetLoadedData(SAVEGAME->PlayerInfo.PlayerLocation,
		SAVEGAME->PlayerInfo.PlayerRotation,
		SAVEGAME->PlayerInfo.bHasBomb,
		SAVEGAME->PlayerInfo.bHasSpeedUp,
		SAVEGAME->PlayerInfo.Bullets,
		SAVEGAME->PlayerInfo.Points,
		SAVEGAME->PlayerInfo.Health);

	TArray<AActor*> AIs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseAI::StaticClass(), AIs);
	for (AActor* Actor : AIs)
	{
		ABaseAI* Bot = Cast<ABaseAI>(Actor);
		FName BotName = Bot->GetFName();
		bool IsDead = true;
		for (int32 i = 0; i < GameInstance->SaveGameObject->BotsInfo.Num(); i++)
		{
			if (SAVEGAME->BotsInfo[i].BotName == BotName)
			{
				Bot->LoadedData(SAVEGAME->BotsInfo[i].bDead, SAVEGAME->BotsInfo[i].Health);
				IsDead = false;
				break;
			}
		}
		if (IsDead)
		{
			Bot->LoadedData(IsDead, 0.f);
		}
	}

	TArray<AActor*> PowerUps;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APowerUp::StaticClass(), PowerUps);
	for (AActor* Actor : PowerUps)
	{
		APowerUp* PowerUp = Cast<APowerUp>(Actor);
		FName PowerUpName = PowerUp->GetFName();
		bool bTaken = true;
		for (int32 i = 0; i < GameInstance->SaveGameObject->PowerUpsInfo.Num(); i++)
		{
			if (SAVEGAME->PowerUpsInfo[i].PowerUpName == PowerUpName)
			{
				PowerUp->LoadedData(false);
				bTaken = false;
				break;
			}
		}
		if (bTaken)
		{
			PowerUp->LoadedData(bTaken);
		}
	}
}