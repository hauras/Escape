
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

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ToggleFlashlightAction;
	// 점프 필요하면 추후 추가 예정
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ToggleMouseModeAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FocusBeamAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InfoScreenClass;

	// 실제로 생성된 정보 창 위젯을 담을 변수.
	UPROPERTY()
	TObjectPtr<UUserWidget> InfoInstance;
	
	void Move(const FInputActionValue& InputAction);
	void Look(const FInputActionValue& InputAction);
	void StartSprint();
	void StopSprint();
	void ToggleFlashLight();
	void Interact();
	void ToggleMouseMode();
	void HandleFocusBeamStarted();
	void HandleFocusBeamCompleted();
	bool bIsMouseMode = false;
};
