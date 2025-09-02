
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EscapeGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnThreatChangedSignature, float, NewValue, float, MaxValue);
/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapeGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnThreatChangedSignature OnThreatChanged;
	
	UFUNCTION(BlueprintPure, Category = "GameState|Threat")
	float GetCurrentThreat() const { return CurrentThreat; }

	UFUNCTION(BlueprintPure, Category = "GameState|Threat")
	float GetMaxThreat() const { return MaxThreat; }
	
	UFUNCTION(BlueprintCallable, Category = "GameState|Threat")
	void AddThreat(float Amount);

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Threat", meta = (AllowPrivateAccess = "true"))
	float CurrentThreat = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameState|Threat", meta = (AllowPrivateAccess = "true"))
	float MaxThreat = 100.f;

	
};
