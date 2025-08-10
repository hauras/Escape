
#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChangedSignature, float, Stamina, float, MaxStamina);
/**
 * 
 */
UCLASS()
class ESCAPE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	APlayerCharacter();

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaChangedSignature OnStaminaChanged;
	
	void StartSprinting();
	void StopSprinting();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	// 스태미나 설정 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;

	// 스태미나 사용량 및 회복량 설정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float StaminaConsumptionRate = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float StaminaRecoveryRate = 15.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float MaxStamina = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed = 600.f;

	

private:
	FTimerHandle StaminaTimerHandle;

	void ConsumeStamina();
	void RecoverStamina();
	
};
