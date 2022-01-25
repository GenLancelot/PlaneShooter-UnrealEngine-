// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAI.h"
#include "PlayerPawn.h"
#include "PlayGameMode.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

void ABaseAI::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (APlayGameMode*)GetWorld()->GetAuthGameMode();
	PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	SetupLocation();
	SetupRotation();
	TraceParams.AddIgnoredActor(this);
}

void ABaseAI::ApplyDamage(float Damage)
{
	Super::ApplyDamage(Damage);

	if (CurrentHealth <= 0.f)
	{
		if (DeathParticle)
		{
			UParticleSystemComponent* Emitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DeathParticle, GetActorLocation(), GetActorRotation());
			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, [Emitter] {Emitter->DestroyComponent(); }, 1.f, false);
		}
		Destroy();
		PlayerPawn->TakePoints(Points);
		GameMode->PawnKilled(false);
	}
}

void ABaseAI::Fire()
{
	Super::Fire();

	bReloading = true;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, [this]() {bReloading = false; }, ReloadTime, false);
}

void ABaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bPaused)
	{
		if (IsGoodToShoot())
		{
			
			Fire();
		}
		if (bMoving)
		{
			MovingTimer += DeltaTime;

			if (MovingTimer > TimeBetweenLocation)
				ResetAndRotate();
			Move();
		}
		if (bRotating)
		{
			RotatingTimer += DeltaTime;
			if (RotatingTimer > TimeBetweenRotation)
			{
				Rotate();
				ResetAndMove();
			}

			Rotate();
		}

	}
}
void ABaseAI::Move()
{
	check(TimeBetweenLocation != 0);
	float ratio = MovingTimer / TimeBetweenLocation;
	if (ratio > 1.0f)
		ratio = 1.0f;
	FVector NewActorLocation;
	if(ToSecondLocation)
		NewActorLocation = FMath::Lerp(FirstLocation, SecondLocation, ratio);
	else
		NewActorLocation = FMath::Lerp(SecondLocation, FirstLocation, ratio);
	SetActorLocation(NewActorLocation);
}

void ABaseAI::ResetAndRotate()
{
	bMoving = false;
	bRotating = true;
	MovingTimer = 0.f;
	RotatingTimer = 0.f;
	ToSecondLocation = !ToSecondLocation;
	SetupRotation();
	Rotate();
}

void ABaseAI::ResetAndMove()
{
	bMoving = true;
	bRotating = false;
	MovingTimer = 0.f;
	RotatingTimer = 0.f;
	SetupRotation();
	Move();
}
void ABaseAI::Rotate()
{
	check(TimeBetweenRotation != 0);
	float ratio = RotatingTimer / TimeBetweenRotation;
	if (ratio > 1.0f)
		ratio = 1.0f;
	FRotator NewActorRotation = FMath::Lerp(FirstRotation, SecondRotation, ratio);
	SetActorRotation(NewActorRotation);
}

void ABaseAI::SetupLocation()
{
	FirstLocation = GetActorLocation();
	FVector Direction = GetActorForwardVector();
	float offset = FMath::RandRange(400.f, 1000.f);
	SecondLocation = FirstLocation + (Direction * offset);
	Direction.X = FMath::Lerp(0.f, 360.f, Direction.X);
	Direction.Y = FMath::Lerp(0.f, 360.f, Direction.Y);

}

void ABaseAI::SetupRotation()
{
	FirstRotation = GetActorRotation();
	SecondRotation = FRotator(0.f, FirstRotation.Yaw - 180.f, 0.f);
}

void ABaseAI::LoadedData(bool IsDead, float Health)
{
	if (IsDead)
	{
		Destroy();
	}
	else
	{
		CurrentHealth = Health;
	}
}

void ABaseAI::LoadedData(bool IsDead)
{
	Destroy();
}

bool ABaseAI::IsGoodToShoot()
{
	FVector Direction = GetActorForwardVector() * -1;
	
	FHitResult Hit;
	if (!IsRealoading() && IsInRange(PlayerPawn->GetActorLocation()) && IsInAimRadius(Direction))
	{
		return true;
	}
	return false;
}

bool ABaseAI::IsInAimRadius(FVector Direction)
{
	FRotator Angle(10.f,10.f, 10.f);
	FVector Direction1 = Angle.RotateVector(Direction);
	Angle = FRotator(350.f, 350.f, 350.f);
	FVector Direction2 = Angle.RotateVector(Direction); Direction2.Normalize();
	Angle = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), PlayerPawn->GetActorLocation());
	FVector Direction3 = Angle.Vector(); Direction3.Normalize();
#if 0
	UE_LOG(LogTemp, Warning, TEXT("Dir: %s    DIR1:%s    DIR2:%s   DIR3:%s"), *(Direction.ToString()), *(Direction1.ToString()), *Direction2.ToString(), *Direction3.ToString());
	FVector StartLocation = GetActorLocation();
	FVector TargetLocation = StartLocation;
	TargetLocation.X += Direction1.X * 1000.f;
	TargetLocation.Y += Direction1.Y * 1000.f;
	DrawDebugLine(GetWorld(), StartLocation, TargetLocation, FColor::Red);
	TargetLocation = StartLocation;
	TargetLocation.X += Direction2.X * 1000.f;
	TargetLocation.Y += Direction2.Y * 1000.f;
	DrawDebugLine(GetWorld(), StartLocation, TargetLocation, FColor::Red);
	TargetLocation = StartLocation;
	TargetLocation.X += Direction3.X * 1000.f;
	TargetLocation.Y += Direction3.Y * 1000.f;
	DrawDebugLine(GetWorld(), StartLocation, TargetLocation, FColor::Red);
#endif 
	
	return (IsValueBetweenTwoOtherValues(Direction3.X, Direction1.X, Direction2.X) && IsValueBetweenTwoOtherValues(Direction3.Z, Direction1.Z, Direction2.Z) && IsValueBetweenTwoOtherValues(Direction3.Y, Direction1.Y, Direction2.Y));
}

bool ABaseAI::IsInRange(FVector PlayerLocation)
{
	return (PlayerLocation - GetActorLocation()).Size() < 1500.f ? true : false;
}
