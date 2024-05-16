// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameGameMode.h"
#include "TagGameCharacter.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ATagGameGameMode::ATagGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATagGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	ResetMatch();
}

void ATagGameGameMode::ResetMatch()
{
	TargetPoints.Empty();
	Balls.Empty();

	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		TargetPoints.Add(*It);
	}

	for (TActorIterator<ABall> It(GetWorld()); It; ++It)
	{
		Balls.Add(*It);
	}

	TArray<ATargetPoint*> RandomTargetPoints = TargetPoints;

	for (int32 Index = 0; Index < Balls.Num(); Index++)
	{
		const int32 TargetToRemoveIndex = FMath::RandRange(0, RandomTargetPoints.Num() - 1);
		const ATargetPoint* Target = RandomTargetPoints[TargetToRemoveIndex];
		RandomTargetPoints.RemoveAt(TargetToRemoveIndex);
		
		Balls[Index]->SetActorLocation(Target->GetActorLocation());
	}
}
