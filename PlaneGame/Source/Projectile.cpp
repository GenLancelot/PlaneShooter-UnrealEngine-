// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/ActorComponent.h"
#include "BaseAI.h"
#include "PlayerPawn.h"



// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	
	SetRootComponent(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 10 * MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;
	InitialLifeSpan = 3.f;

	ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Trail"));
	SHOWREEL_ATTACH(ParticleTrail, ProjectileMesh);

	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySound2D(GetWorld(), LaunchSound);
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//#TODO(jak zostanie czasu): add tags to bots and targets(tarcze albo dolki do zrzutu) 
	//check if otheractor has proper tag and do some operation for example, remove health from Bot, or check value from target

	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->ActorHasTag("BOT"))
		{
			ABaseAI* Bot = Cast<ABaseAI>(OtherActor);
			Bot->ApplyDamage(Damage);
		}
		else if (OtherActor->ActorHasTag("TARGET"))
		{

		}
		else if (OtherActor->ActorHasTag("Player"))
		{
			APlayerPawn* Player = Cast<APlayerPawn>(OtherActor);
			Player->ApplyDamage(Damage);
		}
		UParticleSystemComponent* Emitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation());
		FTimerHandle UnusedHandle;
		UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
		GetWorldTimerManager().SetTimer(UnusedHandle, [Emitter] {Emitter->DestroyComponent();}, 0.5f, false);
		Destroy();
	}
}
