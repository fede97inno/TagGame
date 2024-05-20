// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_LaunFindPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LaunFindPlayer::UBTTask_LaunFindPlayer()
{
	NodeName = TEXT("FindPlayer");	// Name of the Node
}

EBTNodeResult::Type UBTTask_LaunFindPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* Player = AIController->GetWorld()->GetFirstPlayerController()->GetPawn();
	const APawn* AIPawn{ AIController->GetPawn() };
	const FVector AIOrigin{ AIPawn->GetActorLocation() };
	const FVector PlayerOrigin{ Player->GetActorLocation() };
	FVector Direction = PlayerOrigin - AIOrigin;
	Direction.Normalize();
	
	if (FVector::Distance(PlayerOrigin, AIOrigin) < SearchForPlayerRadius)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, Player);
		AIController->GetBlackboardComponent()->SetValueAsVector(FName("ShootDirection"), Direction);
	}
	else
	{
		AIController->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, nullptr);
		AIController->GetBlackboardComponent()->SetValueAsVector(FName("ShootDirection"), FVector::ZeroVector);
	}
	return EBTNodeResult::Succeeded;
}

FString UBTTask_LaunFindPlayer::GetStaticDescription() const
{
	return FString();
}
