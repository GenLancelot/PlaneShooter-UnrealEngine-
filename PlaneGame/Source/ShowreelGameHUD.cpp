// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowreelGameHUD.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.h"

AShowreelGameHUD::AShowreelGameHUD()
{
	
}

void AShowreelGameHUD::DrawHUD()
{
	Super::DrawHUD();

	//to find center
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);


}

void AShowreelGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuWidgetClass);

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}
	}
	
}

