#include "Character/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"
#include "Components/SpotLightComponent.h"
#include "Interface/InteractInterface.h"
#include "DrawDebugHelpers.h" // 디버깅을 위해 추가
#include "Character/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true; 

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
	Spotlight->Intensity = 50000.f; // 빛의 세기 (나중에 조절)
	Spotlight->OuterConeAngle = 35.f; // 빛의 각도 (나중에 조절)
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetStamina(MaxStamina);
	CurrentBattery = MaxBattery; // 배터리 가득 채우고 시작

	if (Spotlight)
	{
		DefaultIntensity = Spotlight->Intensity;
		DefaultConeAngle = Spotlight->OuterConeAngle;
		DefaultColor = Spotlight->LightColor;

	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 상호작용 가능한 오브젝트를 찾는 Trace는 계속 실행합니다.
	TraceForInteractable();

	// --- 손전등 빔 공격 로직 (최종 버전) ---
	if (bIsFocusingBeam && bIsFlashlightOn)
	{
		FVector StartLocation = Spotlight->GetComponentLocation();
		FVector EndLocation = StartLocation + (Spotlight->GetForwardVector() * 1000.f); // 사거리는 10미터
		FHitResult HitResult;

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility);

		AEnemyCharacter* FoundEnemy = bHit ? Cast<AEnemyCharacter>(HitResult.GetActor()) : nullptr;

		// 1. 만약 이번 프레임에 적을 맞췄고, 그 적이 이전에 데미지를 줬던 적과 다르다면 (새로운 타겟)
		if (FoundEnemy && FoundEnemy != LastDamagedEnemy)
		{
			UGameplayStatics::ApplyDamage(FoundEnemy, 1.f, GetController(), this, UDamageType::StaticClass());
		}

		// 2. 현재 조준하고 있는 적을 LastDamagedEnemy 변수에 계속 업데이트합니다.
		LastDamagedEnemy = FoundEnemy;
	}
	else
	{
		// 3. 빔을 쏘고 있지 않다면, '마지막으로 데미지를 준 적'에 대한 기억을 리셋합니다.
		LastDamagedEnemy = nullptr;
	}
}

void APlayerCharacter::ToggleFlashlight()
{

	if (CurrentBattery > 0.f)
	{
		bIsFlashlightOn = !bIsFlashlightOn;
		Spotlight->SetVisibility(bIsFlashlightOn);

	}
	else
	{
		bIsFlashlightOn = false;
		Spotlight->SetVisibility(false);
	}
}

void APlayerCharacter::PerformInteraction()
{
	if (FocusedInteractable)
	{
		IInteractInterface::Execute_Interact(FocusedInteractable.GetObject(), this);
	}
}

void APlayerCharacter::StartFocusingBeam()
{
	bIsFocusingBeam = true;

	if (Spotlight)
	{
		Spotlight->SetIntensity(FocusIntensity);
		Spotlight->SetOuterConeAngle(FocusConeAngle);
		Spotlight->SetLightColor(FocusColor);

		//ToDo : 소리 추가 
	}
}

void APlayerCharacter::StopFocusingBeam()
{
	bIsFocusingBeam = false;
	UE_LOG(LogTemp, Warning, TEXT("Beam FOCUSING stoped."));

	if (Spotlight)
	{
		// Spotlight의 속성을 '원래' 모드로 되돌리기!
		Spotlight->SetIntensity(DefaultIntensity);
		Spotlight->SetOuterConeAngle(DefaultConeAngle);
		Spotlight->SetLightColor(DefaultColor);

		// ToDo: '충전음' 사운드 정지
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

void APlayerCharacter::PickupItem(EItemType ItemType)
{
	if (CurrentItemType == EItemType::EItemType_None)
	{
		CurrentItemType = ItemType;

		if (OnItemChanged.IsBound())
		{
			OnItemChanged.Broadcast(ItemType);
		}
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

void APlayerCharacter::TraceForInteractable()
{
	// ... (StartLocation, EndLocation, HitResult 선언 부분은 동일) ...
	FVector StartLocation;
	FRotator ViewRotation;
	if (!GetController()) return;
	GetController()->GetPlayerViewPoint(StartLocation, ViewRotation);

	const FVector EndLocation = StartLocation + (ViewRotation.Vector() * InteractionDistance);
	FHitResult HitResult;

	// --- 바로 이 부분을 수정합니다 ---

	// [수정 1] 구체의 반지름(크기)을 30.f에서 20.f로 줄였습니다.
	const float SphereRadius = 15.f; 
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		StartLocation,
		EndLocation,
		SphereRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None, 
		HitResult,
		true
	);
	// -----------------------------

	if (bHit && HitResult.GetActor())
	{
		TScriptInterface<IInteractInterface> InteractableActor = HitResult.GetActor();
		if (InteractableActor)
		{
			if (InteractableActor != FocusedInteractable)
			{
				FocusedInteractable = InteractableActor;
			}
			return;
		}
	}
	
	if (FocusedInteractable)
	{
		FocusedInteractable = nullptr;
	}
}