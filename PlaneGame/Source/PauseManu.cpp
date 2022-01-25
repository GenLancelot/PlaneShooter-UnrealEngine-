// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseManu.h"
#include "Kismet/GameplayStatics.h"
#include "PlayGameMode.h"
#include "Components/Button.h"



UPauseManu::UPauseManu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UPauseManu::NativeConstruct()
{
	Super::NativeConstruct();
	ResumeButton->OnPressed.AddDynamic(this, &UPauseManu::Resume);
	ExitButton->OnPressed.AddDynamic(this, &UPauseManu::Leave);
	GameMode = (APlayGameMode*)GetWorld()->GetAuthGameMode();
}

void UPauseManu::Resume()
{
	if (ResumeButton)
	{
		GameMode->SetGamePaused(false);
	}
}

void UPauseManu::Leave()
{
	if (ExitButton)
	{
		GameMode->ParseDataToSave();
		UGameplayStatics::OpenLevel(GetWorld(), MAINMENU);
	}
}