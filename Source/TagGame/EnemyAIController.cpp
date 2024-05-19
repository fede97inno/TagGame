// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "TagGameGameMode.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GoToPlayer = MakeShared<FState>
		(
			[](AAIController* AIController)		// Enter
			{
				AIController->MoveToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), 100.0f);
			},
			nullptr,
			[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FState>		// Tick
			{
				EPathFollowingStatus::Type State = AIController->GetMoveStatus();
				
				if (State == EPathFollowingStatus::Moving /* || BestBall*/)
				{
					return nullptr;
				}

				if (BestBall)
				{
					BestBall->AttachToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
					BestBall->SetActorRelativeLocation(FVector{ 0,0,0 });
					BestBall = nullptr;
				}

				return SearchForBall;
			}
		);

	SearchForBall = MakeShared<FState>
		(
			[this](AAIController* AIController)
			{
				AGameModeBase* GameMode = AIController->GetWorld()->GetAuthGameMode();
				ATagGameGameMode* AIGameMode = Cast<ATagGameGameMode>(GameMode);
				const TArray<ABall*>& Balls = AIGameMode->GetBalls();

				ABall* NearestBall = nullptr;
				for (int32 i = 0; i < Balls.Num(); i++)
				{
					if (!Balls[i]->GetAttachParentActor() && (!NearestBall || FVector::Distance(AIController->GetPawn()->GetActorLocation(), Balls[i]->GetActorLocation()) < FVector::Distance(AIController->GetPawn()->GetActorLocation(), NearestBall->GetActorLocation())))
					{
						NearestBall = Balls[i];
					}
				}

				BestBall = NearestBall;
			},
			nullptr,
			[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FState>
			{
				if (BestBall)
				{
					return GoToBall;
				}

				return SearchForBall;
			}
		);

	GoToBall = MakeShared<FState>
		(
			[this](AAIController* AIController)		// Enter
			{
				AIController->MoveToActor(BestBall, 100.0f);
			},
			nullptr,
			[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FState>		// Tick
			{
				EPathFollowingStatus::Type State = AIController->GetMoveStatus();

				if (State == EPathFollowingStatus::Moving)
				{
					return nullptr;
				}

				return GrabBall;
			}
		);

	GrabBall = MakeShared<FState>
		(
			[this](AAIController* AIController)
			{
				if (BestBall->GetAttachParentActor())
				{
					BestBall = nullptr;
				}
			},
			nullptr,
			[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FState>		// Tick
			{
				if (!BestBall)
				{
					return SearchForBall;
				}
				
				BestBall->SetActorRelativeLocation(FVector{ 0,0,0 });
				BestBall->AttachToActor(AIController->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
				return GoToPlayer;
			}
		);

	CurrentState = SearchForBall;
	CurrentState->CallEnter(this);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState)
	{
		CurrentState = CurrentState->CallTick(this, DeltaTime);
	}
}
