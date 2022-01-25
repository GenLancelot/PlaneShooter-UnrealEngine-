// Fill out your copyright notice in the Description page of Project Settings.


#include "StatisicWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Internationalization/Text.h"


UStatisicWidget::UStatisicWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UStatisicWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Bomb->SetVisibility(ESlateVisibility::Hidden);
	SpeedUp->SetVisibility(ESlateVisibility::Hidden);
}

void UStatisicWidget::UpdateHealth(float Percent)
{
	Health->SetPercent(Percent);
}

void UStatisicWidget::UpdatePoints(float NewPoints)
{
	FString ToSet = "Points: " + FString::SanitizeFloat(NewPoints);
	Points->SetText(FText::FromString(ToSet));
}

void UStatisicWidget::UpdateBullets(int32 Count)
{
	FString ToSet = "Bullets: " + FString::FromInt(Count);
	Bullets->SetText(FText::FromString(ToSet));
}

void UStatisicWidget::UpdateBomb(bool bHave)
{
	if (bHave)
	{
		Bomb->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Bomb->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UStatisicWidget::UpdateSpeedUp(bool bHave)
{
	if (bHave)
	{
		SpeedUp->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SpeedUp->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UStatisicWidget::ShowEndState(bool bPlayerWin)
{
	FString ToSet("You Won! Congratulations!");
	if (!bPlayerWin)
		ToSet = "You Lose! Maybe next time!";
	EndState->SetText(FText::FromString(ToSet));
}