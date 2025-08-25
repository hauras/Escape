

#include "AIController/AIController_Guardian.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIController_Guardian::AAIController_Guardian()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (SightConfig && AIPerceptionComponent)
	{
		SightConfig->SightRadius = 1500.f;
		SightConfig->LoseSightRadius = 2000.f;
		SightConfig->PeripheralVisionAngleDegrees = 45.f;
		SightConfig->SetMaxAge(5.0f); // 5초 동안 기억
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

		AIPerceptionComponent->ConfigureSense(*SightConfig);
		AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	}
}

void AAIController_Guardian::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (IsValid(BehaviorTree))
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		RunBehaviorTree(BehaviorTree);

		if (IsValid(AIPerceptionComponent))
		{
			AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIController_Guardian::OnTargetPerceptionUpdated);
		}
	}
}

void AAIController_Guardian::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (IsValid(Actor) && Cast<APlayerCharacter>(Actor))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"),Actor);
			UE_LOG(LogTemp, Display, TEXT("Target Actor: %s"), *Actor->GetName());
		}
		else
		{
			BlackboardComponent->ClearValue(FName("TargetActor"));
			UE_LOG(LogTemp, Display, TEXT("Target Actor Destroyed"));
		}
	}
}
