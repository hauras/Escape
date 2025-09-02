

#include "Item/DoorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Character/PlayerCharacter.h"

// Sets default values
ADoorBase::ADoorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>("DoorFrame");
	DoorFrame->SetupAttachment(Root);

	Door = CreateDefaultSubobject<UStaticMeshComponent>("Door");
	Door->SetupAttachment(DoorFrame);
}

void ADoorBase::Interact_Implementation(APlayerCharacter* Interactor)
{
	// 플레이어 포인터가 유효하지 않으면 아무것도 하지 않음
	if (!Interactor)
	{
		return;
	}

	// 문이 이미 열려있다면 아무것도 하지 않음 (또는 닫는 로직을 추가할 수 있음)
	if (bIsOpen)
	{
		UE_LOG(LogTemp, Warning, TEXT("Door is already open."));
		return;
	}

	// --- 문이 잠겨 있을 때의 로직 ---
	if (bIsLocked)
	{
		// 플레이어가 필요한 아이템을 가지고 있는지 확인
		if (Interactor->GetCurrentItemType() == RequiredItemType)
		{
			UE_LOG(LogTemp, Log, TEXT("Player has the required key. Unlocking the door."));
			
			// 플레이어의 아이템 사용 처리
			Interactor->UseKeyItem();

			// 문의 상태 변경
			bIsLocked = false;
			bIsOpen = true;

			// 문을 여는 블루프린트 이벤트 호출
			OpenDoor();
		}
		else
		{
			// 필요한 열쇠가 없을 때
			UE_LOG(LogTemp, Warning, TEXT("Door is locked and player does not have the key."));
			
			// TODO: '문이 잠겨있다'는 소리 재생 또는 UI 메시지 표시
		}
	}
	// --- 문이 잠겨있지 않을 때의 로직 ---
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Door is unlocked. Opening the door."));
		
		bIsOpen = true;
		OpenDoor();
	}
}

EInteractState ADoorBase::GetInteractableType_Implementation()
{
	// 이 액터의 상호작용 타입은 '열기' 입니다.
	return EInteractState::Open;
}