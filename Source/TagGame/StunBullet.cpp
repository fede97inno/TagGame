// Fill out your copyright notice in the Description page of Project Settings.

#include "StunBullet.h"
#include "TagGameCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AStunBullet::AStunBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = BulletMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 500.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;
	ProjectileMovementComponent->bShouldBounce = false;
}

// Called when the game starts or when spawned
void AStunBullet::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStunBullet::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Cast<ATagGameCharacter>(OtherActor)->StopMove();
}

// Called every frame
void AStunBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStunBullet::SetVelocity(const FVector& InDirection)
{
	ProjectileMovementComponent->Velocity = InDirection * BulletSpeed;
}

