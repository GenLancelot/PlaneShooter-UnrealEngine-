// Fill out your copyright notice in the Description page of Project Settings.


#include "LeaderboardWidget.h"
#include "SaveLeaderboard.h"
#include "Components/TextBlock.h"
#include "GameInstanceBase.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"



ULeaderboardWidget::ULeaderboardWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void ULeaderboardWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BackButton->OnClicked.AddDynamic(this, &ULeaderboardWidget::ReturnToMainMenu);
	GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));
	GetLeaderboardFromFile();
}

void ULeaderboardWidget::GetLeaderboardFromFile()
{
	FString TableRank = "";
	FString TableName = "";
	FString TableScore = "";
	GameInstance->LoadLeaderboard();
	
	if (LEADERBOARD->IsValidLowLevel())
	{
		if (!LEADERBOARD->Leaderboard.IsEmpty())
		{
			for (int32 i = 0; i < LEADERBOARD->Leaderboard.Num(); i++)
			{
				TableRank += FString::FromInt(LEADERBOARD->Leaderboard[i].rank) + "\n";
				TableName += " " + LEADERBOARD->Leaderboard[i].name + "\n";
				TableScore += " " + FString::SanitizeFloat(LEADERBOARD->Leaderboard[i].points) + "\n";
			}
		}
	}
	SetLeaderboardToTextBlock(TableRank, TableName, TableScore);
}

void ULeaderboardWidget::SetLeaderboardToTextBlock(FString TableRank, FString TableName, FString TableScore)
{
	Rank->SetText(FText::FromString(TableRank));
	Name->SetText(FText::FromString(TableName));
	Score->SetText(FText::FromString(TableScore));
}

void ULeaderboardWidget::ReturnToMainMenu()
{
	if (BackButton)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");
	}
}