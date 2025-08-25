
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_Guardian.generated.h"

class UBehaviorTree;
class UBlackboard;
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
private:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	
};
