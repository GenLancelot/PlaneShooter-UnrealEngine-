// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "PlayerControllerBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "PowerUp.h"
#include "ShowreelGameHUD.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayGameMode.h"



APlayerPawn::APlayerPawn()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(20.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 150.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 5.0f;
	SHOWREEL_ATTACH(SpringArm, RootComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 120.f;
	SHOWREEL_ATTACH(Camera, SpringArm);
	MaxHealth = CurrentHealth = 250.f;
}


void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerPawn::CalculateMoveInput);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerPawn::CalculateRotateInput);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerPawn::FireIfCan);
	PlayerInputComponent->BindAction(TEXT("Bomb"), IE_Pressed, this, &APlayerPawn::DropBomb);
	PlayerInputComponent->BindAction(TEXT("Boost"), IE_Pressed, this, &APlayerPawn::ApplyBoost);
	PlayerInputComponent->BindAction(TEXT("Pause"), IE_Pressed, this, & APlayerPawn::Pause);

}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	GameMode = (APlayGameMode*)GetWorld()->GetAuthGameMode();
	PlayerController = Cast<APlayerControllerBase>(GetWorld()->GetFirstPlayerController());
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);
	UpdateWidget();
}


void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Rotate();
	Move();
}

void APlayerPawn::Pause()
{
	
	if (bPaused)
		return;
	bPaused = true;
	GameMode->SetGamePaused(bPaused);
}

void APlayerPawn::FireIfCan()
{
	if (BulletCount > 0)
		Fire();
}

void APlayerPawn::Fire()
{
	Super::Fire();
	BulletCount--;
	GameMode->UpdateBullets(BulletCount);
}

void APlayerPawn::DetachBomb()
{
	if (BombType)
	{
		FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
		FVector ForwardVector = GetActorForwardVector();
		FRotator SpawnRotation = ForwardVector.Rotation();
		AProjectile* TempBomb = GetWorld()->SpawnActor<AProjectile>(BombType, SpawnLocation, SpawnRotation);
	}
}

void APlayerPawn::DropBomb()
{
	if (!bHaveBomb) return;
	bHaveBomb = false;
	GameMode->UpdateBomb(bHaveBomb);
	DetachBomb();
}


void APlayerPawn::CalculateMoveInput(float Value)
{
	MovementDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APlayerPawn::CalculateRotateInput(float Value)
{
	float RotateAmount = Value * RotationSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation);
}

void APlayerPawn::Move()
{
	AddActorLocalOffset(MovementDirection, true);
}

void APlayerPawn::Rotate()
{
	AddActorLocalRotation(RotationDirection, true);
	FRotator CameraRotation = GetActorRotation();
	CameraRotation.Pitch = -20.f;
	SpringArm->SetRelativeRotation(CameraRotation);
}

void APlayerPawn::ApplyBoost()
{
	if (!bHaveBoost)
		return;
	bHaveBoost = false; 
	GameMode->UpdateSpeedUp(bHaveBoost);
	MoveSpeed += Boost;
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, [this]() {MoveSpeed -= Boost; }, PowerUpTime, false);
}


void APlayerPawn::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("EndGamePlate"))
	{
		
		GameMode->PlayerReachedEndPlate();
	}
	if (OtherActor->ActorHasTag("PowerUps.Bullet") || OtherActor->ActorHasTag("PowerUps.SpeedUp") ||
		OtherActor->ActorHasTag("PowerUps.Points") || OtherActor->ActorHasTag("PowerUps.Bomb") || OtherActor->ActorHasTag("PowerUps.ExtraLife"))
	{
		Cast<APowerUp>(OtherActor)->Take();
		
	}

	

}

void APlayerPawn::ApplyDamage(float Damage)
{
	Super::ApplyDamage(Damage);
	CurrentHealth -= Damage;
	GameMode->UpdateHealth(GetHealthPercent());
	if (CurrentHealth < 0.f)
	{
		GameMode->PawnKilled(true);
		DetachFromControllerPendingDestroy();
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void APlayerPawn::SetLoadedData(FVector LastPlayerLocation, FRotator LastPlayerRotation, bool bHasBomb, bool bHasSpeedUp, int hasBullets, float hasPoints, float hasHealth)
{
	SetActorLocationAndRotation(LastPlayerLocation, LastPlayerRotation);
	bHaveBomb = bHasBomb;
	bHaveBoost = bHasSpeedUp;
	BulletCount = hasBullets;
	Points = hasPoints;
	CurrentHealth = hasHealth;
}

void APlayerPawn::SetLoadedProperties(bool bHasBomb, bool bHasSpeedUp, int hasBullets, float hasPoints, float hasHealth)
{
	bHaveBomb = bHasBomb;
	bHaveBoost = bHasSpeedUp;
	BulletCount = hasBullets;
	Points = hasPoints;
	CurrentHealth = hasHealth;
}

void APlayerPawn::GetPlayerInfo(FVector& LastPlayerLocation, FRotator& LastPlayerRotation, bool& bHasBomb, bool& bHasSpeedUp, int& hasBullets, float& hasPoints, float& hasHealth)
{
	LastPlayerLocation = GetActorLocation();
	LastPlayerRotation = GetActorRotation();
	bHasBomb = bHaveBomb;
	bHasSpeedUp = bHaveBoost;
	hasBullets = BulletCount;
	hasPoints = Points;
	hasHealth = CurrentHealth;
}

void APlayerPawn::UpdateWidget()
{
	GameMode->UpdateSpeedUp(bHaveBoost);
	GameMode->UpdateBomb(bHaveBomb);
	GameMode->UpdateBullets(BulletCount);
	GameMode->UpdatePoints(Points);
}