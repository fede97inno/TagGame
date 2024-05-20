// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_LaunFindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TAGGAME_API UBTTask_LaunFindPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_LaunFindPlayer();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;		// Override for the execution of the Node
	virtual FString GetStaticDescription() const override;	// Can Add a Visual description to the node in Behavior Tree editor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
	float SearchForPlayerRadius{ 1000.0f };
};
