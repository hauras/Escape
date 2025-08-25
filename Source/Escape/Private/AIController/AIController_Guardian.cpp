

#include "AIController/AIController_Guardian.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIController_Guardian::AAIController_Guardian()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AAIController_Guardian::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(BehaviorTree))
	{
		RunBehaviorTree(BehaviorTree);
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}
