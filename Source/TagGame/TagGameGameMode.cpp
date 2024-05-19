// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameGameMode.h"
#include "TagGameCharacter.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ATagGameGameMode::ATagGameGameMode()
{
	// set default pawn class to our Blueprinted character
	PrimaryActorTick.bCanEverTick = true;
	NumberOfBalls = 4;
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATagGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	FString Path = "/Game/Blueprints/BP_Ball.BP_Ball";
	UObject* LoadedBP = StaticLoadObject(UBlueprint::StaticClass(), nullptr, *Path);
	UBlueprint* BP = Cast<UBlueprint>(LoadedBP);

	//UBlueprint* BP = FindObject<UBlueprint>(NULL, *Path);

	if (BP == nullptr || BP->GeneratedClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Blueprints not found"));
		return;
	}

	for (int32 i = 0; i < NumberOfBalls; i++)
	{
		GetWorld()->SpawnActor(BP->GeneratedClass);	
	}

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
		if (It->GetAttachParentActor())
		{
			It->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		}

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

void ATagGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for (int32 i = 0; i < Balls.Num(); i++)
	{
		if (Balls[i]->GetAttachParentActor() != GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			return;
		}
	}

	ResetMatch();
}
