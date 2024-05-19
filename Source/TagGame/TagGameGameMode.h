// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Ball.h"
//#include "BaseEnemyAIController.h"
#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/GameModeBase.h"
#include "TagGameGameMode.generated.h"

UCLASS(minimalapi)
class ATagGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATagGameGameMode();
	void BeginPlay() override;
	void ResetMatch();
	void Tick(float DeltaTime) override;
	const TArray<ABall*>& GetBalls() const { return Balls; }
protected:
	TArray<ATargetPoint*> TargetPoints;
	TArray<ABall*> Balls;
	//TArray<ABaseEnemyAIController*> Enemies;
};