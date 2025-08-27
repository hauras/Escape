

#include "AIController/AIController_Guardian.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/PlayerCharacter.h"
#include "GameState/EscapeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIController_Guardian::AAIController_Guardian()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (SightConfig && AIPerceptionComponent)
	{
		SightConfig->SightRadius = 2000.f;
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
			BlackboardComponent->ClearValue(FName("LastKnownLocation"));

			if (AEscapeGameState* GS = Cast<AEscapeGameState>(UGameplayStatics::GetGameState(this)))
			{
				GS->AddThreat(25.f); // 발각 시 위협 증가
			}
		}
		else
		{
			BlackboardComponent->ClearValue(FName("TargetActor"));
            BlackboardComponent->SetValueAsVector(FName("LastKnownLocation"), Actor->GetActorLocation());
		}
	}
}
