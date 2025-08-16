
#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/EscapeUserWidget.h"
#include "EscapeThreatWidget.generated.h"

class AEscapeGameState;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ESCAPE_API UEscapeThreatWidget : public UEscapeUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	
	UPROPERTY(BlueprintReadOnly, Category="Threat UI") 
	TObjectPtr<UProgressBar> ProgressBar_Threat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Threat")
	float CurrentPercent;
	
	UFUNCTION()
	void OnThreatChanged(float CurrentThreat, float MaxThreat);

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void OnThreatIsFull();

	UFUNCTION(BlueprintImplementableEvent, Category = UI)
	void OnThreatIsNotFull();

private:
	TObjectPtr<AEscapeGameState> GameState;
};
