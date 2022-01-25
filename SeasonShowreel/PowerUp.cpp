// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APowerUp::APowerUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(RootScene);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SHOWREEL_ATTACH(Mesh,RootScene);
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Yaw += 1.f;
	SetActorRotation(ActorRotation);
}

void APowerUp::Take()
{

	PowerUpTakenEvent.Broadcast(PowerUpType);

	Destroy();
}