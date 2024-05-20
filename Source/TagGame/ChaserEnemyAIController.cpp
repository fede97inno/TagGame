// Fill out your copyright notice in the Description page of Project Settings.



// Fill out your copyright notice in the Description page of Project Settings.
#include "ChaserEnemyAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "TagGameGameMode.h"

void AChaserEnemyAIController::InitializeState()
{
	Super::InitializeState();
	CoolDownTimer = SearchCoolDown;
	
	StartingPhase = MakeShared<FState>(
		[this](AAIController* AIController)
		{ 
			SetBestBall(nullptr); 
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FState>
		{
			return SearchForBall;
		}
		);

	GoToPlayer = MakeShared<FState>
		(
			[this](AAIController* AIController)		// Enter
			{
				AIController->MoveToActor(GetPlayer(), 100.0f);
			},
			nullptr,
			[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FState>		// Tick
			{
				EPathFollowingStatus::Type State = AIController->GetMoveStatus();

				if (State == EPathFollowingStatus::Moving)
				{
					return nullptr;
				}
			
				if (GetBestBall())
				{
					GetBestBall()->AttachToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
					GetBestBall()->SetActorRelativeLocation(FVector{0,0,0});
					SetBestBall(nullptr);
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
				SetBestBall(NearestBall);
			},
			nullptr,
			[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FState>
			{
				if (GetBestBall())
				{
					return GoToBall;
				}
				
				CoolDownTimer -= DeltaTime;
				//UE_LOG(LogTemp, Warning, TEXT("%f"), CoolDownTimer);
				
				if (CoolDownTimer <= 0)
				{
					CoolDownTimer = SearchCoolDown;
					return StartingPhase;
				}
				
				return SearchForBall;
			}
			);

	GoToBall = MakeShared<FState>
		(
			[this](AAIController* AIController)		// Enter
			{
				AIController->MoveToActor(GetBestBall(), 100.0f);
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
				if (GetBestBall()->GetAttachParentActor())
				{
					SetBestBall(nullptr);
				}
			},
			nullptr,
			[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FState>		// Tick
			{
				if (!GetBestBall())
				{
					return SearchForBall;
				}

				GetBestBall()->SetActorRelativeLocation(FVector{0,0,0});
				GetBestBall()->AttachToActor(AIController->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
				return GoToPlayer;
			}
			);
}


void AChaserEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	BlackboardComponent = GetPawn()->GetComponentByClass<UBlackboardComponent>();
	SetPlayer();

	if (BlackboardComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finding BlackboardComponent Success"));
	}

	InitializeState();

	CurrentState = SearchForBall;
	CurrentState->CallEnter(this);
}

void AChaserEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState)
	{
		CurrentState = CurrentState->CallTick(this, DeltaTime);
	}
}

ABall* AChaserEnemyAIController::GetBestBall() const
{
	return Cast<ABall>(BlackboardComponent->GetValueAsObject(FName("BestBall")));
}

void AChaserEnemyAIController::SetBestBall(ABall* InBall)
{
	BlackboardComponent->SetValueAsObject(FName("BestBall"), InBall);
}

void AChaserEnemyAIController::SetPlayer()
{
	BlackboardComponent->SetValueAsObject(FName("Player"), GetWorld()->GetFirstPlayerController()->GetPawn());
}

APawn* AChaserEnemyAIController::GetPlayer() const
{
	return Cast<APawn>(BlackboardComponent->GetValueAsObject(FName("Player")));
}