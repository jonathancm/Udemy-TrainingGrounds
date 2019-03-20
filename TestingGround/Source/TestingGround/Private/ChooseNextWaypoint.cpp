// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get Patrol Guard
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
		return EBTNodeResult::Failed;

	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!PatrolRoute)
		return EBTNodeResult::Failed;

	TArray<AActor*> PatrolPoints = PatrolRoute->GetPatrolPoints();
	if(PatrolPoints.Num() == 0)
		return EBTNodeResult::Failed;

	// Set Next Waypoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	int32 Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(PatrolPointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle Index
	Index = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, Index);

	return EBTNodeResult::Succeeded;
}
