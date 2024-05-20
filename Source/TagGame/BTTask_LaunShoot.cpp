// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_LaunShoot.h"
#include "AIController.h"
#include "LauncherEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LaunShoot::UBTTask_LaunShoot()
{
	NodeName = TEXT("Shoot");	// Name of the Node
}

EBTNodeResult::Type UBTTask_LaunShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	FVector Direction = AIController->GetBlackboardComponent()->GetValueAsVector(FName("ShootDirection"));
	if (Direction != FVector::ZeroVector)
	{
		Cast<ALauncherEnemyAIController>(AIController)->Shoot(Direction);
	}

	return EBTNodeResult::Succeeded;
}

FString UBTTask_LaunShoot::GetStaticDescription() const
{
	return FString();
}
