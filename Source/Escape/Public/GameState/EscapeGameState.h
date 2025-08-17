
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

	// --- 2. 접근자 (Getters) ---
	// 블루프린트에서 값을 읽기만 할 수 있도록 BlueprintPure로 선언합니다.
	UFUNCTION(BlueprintPure, Category = "GameState|Threat")
	float GetCurrentThreat() const { return CurrentThreat; }

	UFUNCTION(BlueprintPure, Category = "GameState|Threat")
	float GetMaxThreat() const { return MaxThreat; }

	// --- 3. 변경자 (Mutators) ---
	// 블루프린트에서도 호출할 수 있도록 BlueprintCallable로 선언합니다.
	UFUNCTION(BlueprintCallable, Category = "GameState|Threat")
	void AddThreat(float Amount);

protected:
	// --- 4. 데이터 (Variables) ---
	// 외부에서 직접 수정할 수 없도록 protected로 선언하는 것이 더 안전합니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GameState|Threat", meta = (AllowPrivateAccess = "true"))
	float CurrentThreat = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameState|Threat", meta = (AllowPrivateAccess = "true"))
	float MaxThreat = 100.f;

	
};
