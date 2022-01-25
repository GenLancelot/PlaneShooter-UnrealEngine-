// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "PlayerControllerBase.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/EditableTextBox.h"
#include "GameInstanceBase.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	StartButton->OnClicked.AddDynamic(this, &UMainMenu::StartNewGame);
	ResumeButton->OnClicked.AddDynamic(this, &UMainMenu::ContinueLastGame);
	LeaderboardButton->OnClicked.AddDynamic(this, &UMainMenu::LaunchLeaderboard);
	ExitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	PlayerNameBox->OnTextCommitted.AddDynamic(this, &UMainMenu::UpdatePlayerName);
}

void UMainMenu::StartNewGame()
{
	
	if (StartButton)
	{
		APlayerController* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
		if (PlayerController)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			PlayerController->SetShowMouseCursor(false);
			UGameInstanceBase* GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));
			GameInstance->SetShouldLoadSavedGame(true);
			UGameplayStatics::OpenLevel(GetWorld(), GameInstance->GetCurrentLevelName());
		}
	}
}

void UMainMenu::ContinueLastGame()
{
	
	if (ResumeButton)
	{
		APlayerController* PlayerController = Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
		if (PlayerController)
		{
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
			PlayerController->SetShowMouseCursor(false);
		}
		UGameInstanceBase* GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->SetShouldLoadSavedGame(true);
		UGameplayStatics::OpenLevel(GetWorld(), GameInstance->GetCurrentLevelName());
	}
}

void UMainMenu::LaunchLeaderboard()
{
	if (LeaderboardButton)
	{
		
		UGameplayStatics::OpenLevel(GetWorld(), "LeaderboardLevel");
	}
}

void UMainMenu::QuitGame()
{
	if (ExitButton)
	{

		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
	}
}

void UMainMenu::UpdatePlayerName(const FText& InText, ETextCommit::Type InCommitType)
{
	if (PlayerNameBox)
	{
		
		UGameInstanceBase* GameInstance = Cast<UGameInstanceBase>(UGameplayStatics::GetGameInstance(GetWorld()));
		GameInstance->SetPlayerName(InText.ToString());
	}
}
