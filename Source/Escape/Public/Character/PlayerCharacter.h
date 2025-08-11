#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h" // ACharacterBase를 상속받는 것이 맞다면 그대로 둡니다.
#include "PlayerCharacter.generated.h"

// .h 파일 개선점:
// 1. 클래스에 대한 포인터만 필요할 경우, 전체 헤더를 include하는 대신 '전방 선언'을 사용하면 컴파일 시간이 단축됩니다.
class UCameraComponent;
class USpringArmComponent;

// 2. 델리게이트 이름을 더 명확하게 수정하고, 여기에서만 선언합니다.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaPercentChangedDelegate, float, NewPercent);

UCLASS()
class ESCAPE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	/** UI가 구독할 스태미나 변경 델리게이트입니다. */
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnStaminaPercentChangedDelegate OnStaminaPercentChanged;

	// 스프린트 시작/종료 함수. 블루프린트에서도 호출할 수 있도록 UFUNCTION을 붙입니다.
	UFUNCTION(BlueprintCallable, Category = "Player Action")
	void StartSprinting();

	UFUNCTION(BlueprintCallable, Category = "Player Action")
	void StopSprinting();

protected:
	// 게임 시작 시 호출됩니다.
	virtual void BeginPlay() override;

	// --- 컴포넌트 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;


	// --- 스태미나 관련 프로퍼티 (에디터에서 수정 가능) ---
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats|Stamina")
	float MaxStamina;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats|Stamina")
	float StaminaConsumptionRate; // 초당 소모량

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats|Stamina")
	float StaminaRecoveryRate;  // 초당 회복량
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats|Stamina")
	float StaminaUpdateInterval; // 스태미나 업데이트 주기 (초)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats|Movement")
	float SprintSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats|Movement")
	float WalkSpeed;
	
private:
	// --- 내부 상태 변수 (UPROPERTY 불필요) ---
	float CurrentStamina;
	bool bIsSprinting;

	/** 스태미나 타이머를 관리하기 위한 핸들입니다. */
	FTimerHandle StaminaTimerHandle;

	// --- 내부 로직 함수 ---
	void SetStamina(float NewStamina);
	void ConsumeStamina();
	void RecoverStamina();
};