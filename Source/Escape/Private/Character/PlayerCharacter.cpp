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
#include "Sound/SoundCue.h"
#include "Interface/LightSensesitiveInterface.h"

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
		FVector EndLocation = StartLocation + (Spotlight->GetForwardVector() * 1000.f);
		FHitResult HitResult;

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility);
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, bHit ? FColor::Green : FColor::Red, false, -1, 0, 1.0f);

		if (bHit && HitResult.GetActor())
		{
			// [수정 1] 먼저, 부딪힌 대상이 '빛에 민감한' 인터페이스를 가졌는지 확인합니다.
			if (HitResult.GetActor()->Implements<ULightSensesitiveInterface>())
			{
				// 인터페이스 함수를 호출하여, 빛의 세기와 시간을 전달합니다.
				ILightSensesitiveInterface::Execute_OnLightReceived(HitResult.GetActor(), Spotlight->Intensity, DeltaTime);
			}

			// [수정 2] 기존의 '적 데미지' 로직은 그대로 두어도 좋지만,
			// 이제 OnLightReceived 안에서 처리하는 것이 더 좋은 설계일 수 있습니다.
			// 여기서는 일단 기존 로직을 유지하겠습니다.
			AEnemyCharacter* FoundEnemy = Cast<AEnemyCharacter>(HitResult.GetActor());
			if (FoundEnemy && FoundEnemy != LastDamagedEnemy)
			{
				UGameplayStatics::ApplyDamage(FoundEnemy, 1.f, GetController(), this, UDamageType::StaticClass());
			}
			LastDamagedEnemy = FoundEnemy;
		}
		else
		{
			LastDamagedEnemy = nullptr;
		}
	}
	else
	{
		LastDamagedEnemy = nullptr;
	}

	if (bIsFlashlightOn && CurrentBattery > 0.f)
	{
		const float OldBattery = CurrentBattery;
		CurrentBattery -= BatteryConsumptionRate * DeltaTime;
		CurrentBattery = FMath::Max(CurrentBattery, 0.f); // 0 밑으로 내려가지 않게

		// 배터리 값에 변화가 생겼다면, UI에게 방송!
		if (OldBattery != CurrentBattery && OnBatteryChanged.IsBound())
		{
			OnBatteryChanged.Broadcast(CurrentBattery, MaxBattery);
		}

		// 배터리가 다 닳으면 손전등을 강제로 끕니다.
		if (CurrentBattery <= 0.f)
		{
			ToggleFlashlight();
		}
	}
}

void APlayerCharacter::ToggleFlashlight()
{
	const bool bWasOn = bIsFlashlightOn;
	
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

	if (bWasOn != bIsFlashlightOn)
	{
		if (bIsFlashlightOn)
		{
			if (IsValid(FlashLightSound))
			{
				UGameplayStatics::PlaySoundAtLocation(this , FlashLightSound, GetActorLocation());
			}
		}
		else
		{
			if (IsValid(FlashLightSound))
			{
				UGameplayStatics::PlaySoundAtLocation(this , FlashLightSound, GetActorLocation());
			}
		}
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
		Spotlight->SetAttenuationRadius(FocusAttenuationRadius);
	}
}

void APlayerCharacter::StopFocusingBeam()
{
	bIsFocusingBeam = false;

	if (Spotlight)
	{
		// Spotlight의 속성을 '원래' 모드로 되돌리기!
		Spotlight->SetIntensity(DefaultIntensity);
		Spotlight->SetOuterConeAngle(DefaultConeAngle);
		Spotlight->SetLightColor(DefaultColor);

	}
}

void APlayerCharacter::UseKeyItem()
{
	CurrentItemType = EItemType::EItemType_None;

	if (OnItemChanged.IsBound())
	{
		OnItemChanged.Broadcast(CurrentItemType);
	}
	UE_LOG(LogTemp, Warning, TEXT("Item changed."));
}

void APlayerCharacter::SetStamina(float NewStamina)
{
	const float OldStamina = CurrentStamina;
	CurrentStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

	if (OldStamina != CurrentStamina && OnStaminaPercentChanged.IsBound())
	{
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
	FVector StartLocation;
	FRotator ViewRotation;
	if (!GetController()) return;
	GetController()->GetPlayerViewPoint(StartLocation, ViewRotation);

	const FVector EndLocation = StartLocation + (ViewRotation.Vector() * InteractionDistance);
	FHitResult HitResult;


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