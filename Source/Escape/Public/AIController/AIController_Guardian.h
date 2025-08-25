
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Guardian.generated.h"

class UBehaviorTree;
class UBlackboard;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
/**
 * 
 */
UCLASS()
class ESCAPE_API AAIController_Guardian : public AAIController
{
	GENERATED_BODY()
public:
	AAIController_Guardian();

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;
private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
