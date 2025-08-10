
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EscapeController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class ESCAPE_API AEscapeController : public APlayerController
{
	GENERATED_BODY()
public:
	AEscapeController();
protected:
	virtual void BeginPlay() override;

	virtual auto SetupInputComponent() -> void override;
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;
	
	// 점프 필요하면 추후 추가 예정
	
	void Move(const FInputActionValue& InputAction);
	void Look(const FInputActionValue& InputAction);
	void StartSprint();
	void StopSprint();

};
