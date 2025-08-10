

#include "Character/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
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
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;


}

void APlayerCharacter::StartSprinting()
{
	if (CurrentStamina > 0.f)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &APlayerCharacter::ConsumeStamina, 0.1f, true);
	}
}

void APlayerCharacter::StopSprinting()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &APlayerCharacter::RecoverStamina, 0.1f, true);

}

void APlayerCharacter::ConsumeStamina()
{
	const float OldStamina = CurrentStamina;
	CurrentStamina -= StaminaConsumptionRate * 0.1f;

	if (CurrentStamina <= 0.f)
	{
		CurrentStamina = 0.f;
		StopSprinting();
	}

	if (OldStamina != CurrentStamina)
	{
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
	}
}

void APlayerCharacter::RecoverStamina()
{
	const float OldStamina = CurrentStamina;
	CurrentStamina += StaminaRecoveryRate * 0.1f;

	if (CurrentStamina >= MaxStamina)
	{
		CurrentStamina = MaxStamina;
		// 스태미나가 가득 차면 더 이상 회복할 필요가 없으므로 타이머 중지
		GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
	}
    
	// 값 변경 시 델리게이트 호출
	if (OldStamina != CurrentStamina)
	{
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina); // TwoParams 버전으로 수정
	}
}
