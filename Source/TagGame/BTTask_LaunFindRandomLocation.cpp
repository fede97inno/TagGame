// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LaunFindRandomLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

UBTTask_LaunFindRandomLocation::UBTTask_LaunFindRandomLocation()
{
	NodeName = TEXT("FindRandomLocation");	// Name of the Node

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_LaunFindRandomLocation, BlackboardKey)); // Filter the blackboard to accept only vector for this task.
}

EBTNodeResult::Type UBTTask_LaunFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location{};

	AAIController* AIController = OwnerComp.GetAIOwner();		// Get AIPawn to calculate the new Location
	const APawn* AIPawn {AIController->GetPawn()};
	const FVector Origin {AIPawn->GetActorLocation()};

	const UNavigationSystemV1* NavSystem{ UNavigationSystemV1::GetCurrent(GetWorld()) };	// Get the current NavSystem

	if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);		// Signal the BehaviorTreeComponent the task finished with success, Call the OnFinishTask

	return EBTNodeResult::Succeeded;
}

FString UBTTask_LaunFindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"), *BlackboardKey.SelectedKeyName.ToString());
}
