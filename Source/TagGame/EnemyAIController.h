// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ball.h"
#include "EnemyAIController.generated.h"

struct FState : public TSharedFromThis<FState>		// Add AsShared we can use it as this
{
private:
	TFunction<void(AAIController*)> Enter;	// return type(arguments)
	TFunction<void(AAIController*)> Exit;
	TFunction<TSharedPtr<FState>(AAIController*, const float)> Tick;	// If FAivState is null remain in this state, in the other case we change

public:
	FState()
	{
		Enter = nullptr;
		Exit = nullptr;
		Tick = nullptr;
	}

	FState(TFunction<void(AAIController*)> InEnter = nullptr, TFunction<void(AAIController*)> InExit = nullptr, TFunction<TSharedPtr<FState>(AAIController*, const float)> InTick = nullptr)
		:
		Enter{InEnter},
		Exit{InExit}, 
		Tick{InTick}
	{

	}

	FState(const FState& Other) = delete;
	FState& operator=(const FState& Other) = delete;
	FState(FState&& Other) = delete;
	FState& operator=(const FState&& Other) = delete;

	void CallEnter(AAIController* AIController) 
	{
		if (Enter)
		{
			Enter(AIController);
		}
	}

	void CallExit(AAIController* AIController)
	{
		if (Exit)
		{
			Exit(AIController);
		}
	}

	TSharedPtr<FState> CallTick(AAIController* AIController, const float DeltaTime)
	{
		if (Tick)
		{
			TSharedPtr<FState> NewState = Tick(AIController, DeltaTime);

			if (NewState != nullptr && NewState != AsShared())
			{
				CallExit(AIController);
				NewState->CallEnter(AIController);
				return NewState;
			}
		}

		return AsShared();
	}
};

/**
 * 
 */
UCLASS()
class TAGGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
protected:
	TSharedPtr<FState> CurrentState;
	TSharedPtr<FState> GoToPlayer;
	TSharedPtr<FState> SearchForBall;
	TSharedPtr<FState> GoToBall;
	TSharedPtr<FState> GrabBall;
public:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	ABall* BestBall;
};
