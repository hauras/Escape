#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h" 
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USpotLightComponent;
class UStaticMeshComponent;
class IInteractInterface; 
class AEnemyCharacter; // 전방 선언 추가

UENUM(BlueprintType)
enum class EItemType : uint8
{
	EItemType_None UMETA(DisplayName = "None"),
	EItemType_Key UMETA(DisplayName = "Key"),

	// 추 후 아이템 추가 
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaPercentChangedDelegate, float, NewPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatteryChangedSignature, float, CurrentValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemChangedSignature, EItemType, NewItemType);

UCLASS()
class ESCAPE_API APlayerCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnStaminaPercentChangedDelegate OnStaminaPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnBatteryChangedSignature OnBatteryChanged;

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnItemChangedSignature OnItemChanged;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PickupItem(EItemType ItemType);
	// 스프린트 시작/종료 함수. 블루프린트에서도 호출할 수 있도록 UFUNCTION을 붙입니다.
	UFUNCTION(BlueprintCallable, Category = "Player Action")
	void StartSprinting();

	UFUNCTION(BlueprintCallable, Category = "Player Action")
	void StopSprinting();

	UFUNCTION(BlueprintCallable, Category = "Player Action")
	void ToggleFlashlight();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PerformInteraction();

    UFUNCTION(BlueprintCallable, Category = "Player Action|Flashlight")
	void StartFocusingBeam();

	UFUNCTION(BlueprintCallable, Category = "Player Action|Flashlight")
	void StopFocusingBeam();

protected:
	// 게임 시작 시 호출됩니다.
	virtual auto BeginPlay() -> void override;
	virtual void Tick(float DeltaTime) override;

	// --- 컴포넌트 ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	TObjectPtr<UStaticMeshComponent> FlashlightMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	TObjectPtr<USpotLightComponent> Spotlight;
	
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	bool bIsFlashlightOn = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flashlight|Battery")
	float MaxBattery = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flashlight|Battery")
	float CurrentBattery;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flashlight|Battery")
	float BatteryConsumptionRate = 2.f;

	// 적 공격 빛 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flashlight|Focus Beam")
	float FocusIntensity = 20000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flashlight|Focus Beam")
	float FocusConeAngle = 5.f;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flashlight|Focus Beam")
	FColor FocusColor = FColor::Cyan;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	EItemType CurrentItemType = EItemType::EItemType_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flashlight")
	bool bIsFocusingBeam = false;
	
private:
	float CurrentStamina;
	bool bIsSprinting;

	float DefaultIntensity;
	float DefaultConeAngle;
	FColor DefaultColor;
	
	FTimerHandle StaminaTimerHandle;

	void SetStamina(float NewStamina);
	void ConsumeStamina();
	void RecoverStamina();

	void TraceForInteractable();

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float InteractionDistance = 400.f;

	UPROPERTY(VisibleAnywhere)
	TScriptInterface<IInteractInterface> FocusedInteractable;

	UPROPERTY()
	TObjectPtr<AEnemyCharacter> LastDamagedEnemy;
};