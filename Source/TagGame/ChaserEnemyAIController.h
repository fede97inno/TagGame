// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ChaserEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TAGGAME_API AChaserEnemyAIController : public ABaseEnemyAIController
{
	GENERATED_BODY()
protected:
	TSharedPtr<FState> StartingPhase;
	TSharedPtr<FState> GoToPlayer;
	TSharedPtr<FState> SearchForBall;
	TSharedPtr<FState> GoToBall;
	TSharedPtr<FState> GrabBall;
	const float SearchCoolDown{ 5.0f };
	float CoolDownTimer;
public:
	ABall* GetBestBall() const;
	void SetBestBall(ABall* InBall);
	
	APawn* GetPlayer() const;
	void SetPlayer();

	void InitializeState() override;
	void BeginPlay() override;
	void Tick(const float DeltaTime) override;
	
	UBlackboardComponent* BlackboardComponent;
};
