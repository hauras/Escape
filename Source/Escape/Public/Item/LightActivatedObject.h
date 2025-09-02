
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/LightSensesitiveInterface.h"
#include "LightActivatedObject.generated.h"

UCLASS()
class ESCAPE_API ALightActivatedObject : public AActor, public ILightSensesitiveInterface
{
	GENERATED_BODY()
	
public:	
	ALightActivatedObject();

	virtual void OnLightReceived_Implementation(float Intensity, float DeltaTime) override;
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle")
	TObjectPtr<AActor> TargetActor;

	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle")
	void ActivateStairs();

private:
	UPROPERTY()
	bool bHasActivated = false;
	
};
