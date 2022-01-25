// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "PlayGameMode.h"
#include "DrawDebugHelpers.h"



// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    SetRootComponent(Capsule);
    Capsule->SetSimulatePhysics(true);
    Capsule->SetEnableGravity(false);
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SHOWREEL_ATTACH(Mesh, Capsule);
    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
    SHOWREEL_ATTACH(ProjectileSpawnPoint, Mesh);

    TraceParams.AddIgnoredActor(this);
}



void ABasePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FRotator ActorRotation = GetActorRotation();
    if (ActorRotation.Roll || ActorRotation.Pitch)
    {
        ActorRotation.Roll = 0.f;
        ActorRotation.Pitch = 0.f;
        SetActorRotation(ActorRotation);
    }
    FHitResult Hit;
    ActorLocation = GetActorLocation();
    FVector TargetLocation = ActorLocation;
    TargetLocation.Z -= 10000.f;
    bool IsHit = World->LineTraceSingleByChannel(Hit, ActorLocation, TargetLocation, ECollisionChannel::ECC_WorldStatic, TraceParams);
    if (IsHit)
    {
        ActorLocation.Z = Hit.Location.Z + LocationZ;
        SetActorLocation(ActorLocation);
    }
    
}

void ABasePawn::ApplyDamage(float Damage)
{
    CurrentHealth -= Damage;
}

void ABasePawn::BeginPlay()
{
    Super::BeginPlay();
    World = GetWorld();
    
    ActorLocation = GetActorLocation();
    LocationZ = ActorLocation.Z;
}

void ABasePawn::Fire()
{
   
    if (ProjectileType)
    {
        FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
        FVector ForwardVector = GetActorForwardVector();
        if (this->ActorHasTag("BOT"))
            ForwardVector *= -1;
        FRotator SpawnRotation = ForwardVector.Rotation();
        AProjectile* TempProjectile = World->SpawnActor<AProjectile>(ProjectileType, SpawnLocation, SpawnRotation);
    }
    
}

float ABasePawn::GetHealthPercent()
{
    float Percent = 0;

    try {
        if (MaxHealth == 0)
        {
            throw "Division by zero condition!";
        }
        Percent = CurrentHealth / MaxHealth;
    }
    catch (const char* msg) {
        UE_LOG(LogTemp, Warning, TEXT("%s"), msg);
    }
    return Percent;
}