// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseEnemyAIController.h"
#include "LauncherEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TAGGAME_API ALauncherEnemyAIController : public ABaseEnemyAIController
{
	GENERATED_BODY()
	
public:
	ALauncherEnemyAIController();
	void Shoot(FVector InDirection);
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))	// AllowPrivateAccess = true permit the access to the property from blueprint even if is private
	UBehaviorTree* BehaviorTree;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))	
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = true))
	UBlackboardComponent* BlackboardComponent;
};
