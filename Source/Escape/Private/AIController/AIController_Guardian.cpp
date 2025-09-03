

#include "AIController/AIController_Guardian.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EnemyCharacter.h"
#include "Character/PlayerCharacter.h"
#include "GameState/EscapeGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIController_Guardian::AAIController_Guardian()
{
	PrimaryActorTick.bCanEverTick = true;

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

void AAIController_Guardian::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(GetBlackboardComponent()))
	{
		// 블랙보드의 'TargetActor' 키에 유효한 값이 있는지(즉, 추격 중인지) 확인합니다.
		if (IsValid(GetBlackboardComponent()->GetValueAsObject(FName("TargetActor"))))
		{
			// GameState를 가져옵니다.
			if (AEscapeGameState* GS = Cast<AEscapeGameState>(UGameplayStatics::GetGameState(this)))
			{
				// 초당 5.0 (값은 나중에 조절)의 속도로 위협도를 '지속적으로' 증가시킵니다.
				GS->AddThreat(5.0f * DeltaTime); 
			}
		}
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
			AEscapeGameState* GS = Cast<AEscapeGameState>(UGameplayStatics::GetGameState(this));
			if (IsValid(GS))
			{
				GS->AddThreat(3.f);
			}
			else
			{
				
			}
		}
		else
		{
			BlackboardComponent->ClearValue(FName("TargetActor"));
            BlackboardComponent->SetValueAsVector(FName("LastKnownLocation"), Actor->GetActorLocation());
		}
	}
}
