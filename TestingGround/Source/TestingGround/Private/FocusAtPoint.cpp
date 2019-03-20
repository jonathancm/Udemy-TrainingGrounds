// Fill out your copyright notice in the Description page of Project Settings.

#include "FocusAtPoint.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


EBTNodeResult::Type UFocusAtPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Patrol Guard
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return EBTNodeResult::Failed;

	// Set Focal Point
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	//FVector FocusPoint = BlackboardComp->GetValueAsVector(FocusPointKey.SelectedKeyName);
	FVector FocusPoint = FVector::ZeroVector;
	AIController->SetFocalPoint(FocusPoint);

	return EBTNodeResult::Succeeded;
}
