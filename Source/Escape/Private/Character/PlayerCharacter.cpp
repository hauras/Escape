#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Components/SpotLightComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false; 

	// --- 컴포넌트 설정 ---
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	MaxStamina = 100.f;
	StaminaConsumptionRate = 20.f;
	StaminaRecoveryRate = 15.f;
	StaminaUpdateInterval = 0.1f;
	SprintSpeed = 600.f;
	WalkSpeed = 400.f; 
	
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	FlashlightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightMesh"));
    FlashlightMesh->SetupAttachment(GetMesh(), FName("FlashlightSocket"));

	Spotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight"));
	Spotlight->SetupAttachment(FollowCamera);
	Spotlight->SetVisibility(false); // 처음에는 꺼진 상태로 시작
	Spotlight->Intensity = 5000.f; // 빛의 세기 (나중에 조절)
	Spotlight->OuterConeAngle = 25.f; // 빛의 각도 (나중에 조절)
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetStamina(MaxStamina);
	CurrentBattery = MaxBattery; // 배터리 가득 채우고 시작

}

void APlayerCharacter::ToggleFlashlight()
{
	UE_LOG(LogTemp, Warning, TEXT("3. Character::ToggleFlashlight() Called. CurrentBattery is %f"), CurrentBattery);

	if (CurrentBattery > 0.f)
	{
		bIsFlashlightOn = !bIsFlashlightOn;
		Spotlight->SetVisibility(bIsFlashlightOn);
		UE_LOG(LogTemp, Warning, TEXT("4. Flashlight visibility set to: %s"), bIsFlashlightOn ? TEXT("True") : TEXT("False"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Flashlight NOT turned on. Battery is empty."));
		bIsFlashlightOn = false;
		Spotlight->SetVisibility(false);
	}
}

// 스태미나를 변경하고 UI에 알리는 유일한 창구
void APlayerCharacter::SetStamina(float NewStamina)
{
	const float OldStamina = CurrentStamina;
	CurrentStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

	if (OldStamina != CurrentStamina && OnStaminaPercentChanged.IsBound())
	{
		// 3. 델리게이트 이름과 파라미터를 헤더와 통일합니다.
		const float StaminaPercent = MaxStamina > 0.f ? CurrentStamina / MaxStamina : 0.f;
		OnStaminaPercentChanged.Broadcast(StaminaPercent);
	}
}

void APlayerCharacter::StartSprinting()
{
	if (CurrentStamina > 0.f)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &APlayerCharacter::ConsumeStamina, StaminaUpdateInterval, true);
	}
}

void APlayerCharacter::StopSprinting()
{
	bIsSprinting = false;
	// 4. '매직 넘버' 대신 변수를 사용합니다.
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &APlayerCharacter::RecoverStamina, StaminaUpdateInterval, true);
}

void APlayerCharacter::ConsumeStamina()
{
	SetStamina(CurrentStamina - StaminaConsumptionRate * StaminaUpdateInterval);
	if (CurrentStamina <= 0.f)
	{
		StopSprinting(); 
	}
}

void APlayerCharacter::RecoverStamina()
{
	SetStamina(CurrentStamina + StaminaRecoveryRate * StaminaUpdateInterval);
	if (CurrentStamina >= MaxStamina)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
	}
}