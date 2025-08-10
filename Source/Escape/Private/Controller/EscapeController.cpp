

#include "Controller/EscapeController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Character/PlayerCharacter.h"


AEscapeController::AEscapeController()
{
}

void AEscapeController::BeginPlay()
{
	Super::BeginPlay();
	check(InputContext);

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(SubSystem);
	SubSystem->AddMappingContext(InputContext, 0);
	
}

void AEscapeController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, & AEscapeController::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, & AEscapeController::Look);

	if (SprintAction)
	{
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, & AEscapeController::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, & AEscapeController::StopSprint);

	}

}

void AEscapeController::Move(const FInputActionValue& InputAction)
{
	const FVector2D InputAxisVector = InputAction.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AEscapeController::Look(const FInputActionValue& InputAction)
{
	const FVector2D LookAxisVector = InputAction.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		// 마우스의 X축 움직임(좌우)은 컨트롤러의 Yaw 회전에,
		// 마우스의 Y축 움직임(상하)은 컨트롤러의 Pitch 회전에 영향을 줍니다.
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AEscapeController::StartSprint()
{
	if (APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->StartSprinting();
	}
}

void AEscapeController::StopSprint()
{
	if (APlayerCharacter* PlayerCharacter = GetPawn<APlayerCharacter>())
	{
		PlayerCharacter->StopSprinting();
	}
}
