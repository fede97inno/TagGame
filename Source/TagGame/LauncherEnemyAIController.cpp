// Fill out your copyright notice in the Description page of Project Settings.

#include "LauncherEnemyAIController.h"
#include "StunBullet.h"
#include "BehaviorTree/BehaviorTree.h"

ALauncherEnemyAIController::ALauncherEnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ALauncherEnemyAIController::Shoot(FVector InDirection)
{
	FString Path = "/Game/Blueprints/BP_StunBullet.BP_StunBullet";
	UObject* LoadedBP = StaticLoadObject(UBlueprint::StaticClass(), nullptr, *Path);
	UBlueprint* BP = Cast<UBlueprint>(LoadedBP);


	AActor* SpawnedActor = GetWorld()->SpawnActor(BP->GeneratedClass);
	SpawnedActor->SetActorLocation(GetPawn()->GetActorLocation() + FVector{10.0f,10.0f,10.0f});
	Cast<AStunBullet>(SpawnedActor)->SetVelocity(InDirection);

	UE_LOG(LogTemp, Warning, TEXT("DIRECTOOOOO SHOOOOOOOOOOOOOOOOTTOOOOOOOO"));
}

void ALauncherEnemyAIController::BeginPlay()
{
	Super::BeginPlay();


	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void ALauncherEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (Blackboard && BehaviorTree)
	{
		Blackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}
