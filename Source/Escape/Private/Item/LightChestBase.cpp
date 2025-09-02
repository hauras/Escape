

#include "Item/LightChestBase.h"
#include "Components/BoxComponent.h" 

ALightChestBase::ALightChestBase()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName("BoxCollision"));
	RootComponent = BoxCollision;

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChestMesh"));
	ChestMesh->SetupAttachment(RootComponent);

	// 상호작용 Trace가 감지할 수 있도록 콜리전 설정을 해줍니다.
	ChestMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ALightChestBase::Interact_Implementation(APlayerCharacter* Interactor)
{
	if (bIsUnLocked)
	{
        UE_LOG(LogTemp, Warning, TEXT("Chest Opened!"));
	}
}

EInteractState ALightChestBase::GetInteractableType_Implementation()
{
	return EInteractState::Open;
}

void ALightChestBase::OnLightReceived_Implementation(float Intensity, float DeltaTime)
{    UE_LOG(LogTemp, Warning, TEXT("[Chest Log] SUCCESS: OnLightReceived function was EXECUTED! Current Charge: %f"), CurrentCharge);

	if (bIsUnLocked || CurrentCharge >= MaxCharge) return;
	CurrentCharge += (Intensity / 1000.f) * DeltaTime;
    
	// 블루프린트에 현재 충전율(0~1)을 알림
	OnChargeUpdated(CurrentCharge / MaxCharge);
    
	if (CurrentCharge >= MaxCharge)
	{
		bIsUnLocked = true;
		OnUnlocked(); // 잠금 해제 이벤트 호출!
	}
}

