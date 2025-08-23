

#include "Item/KeyItemBase.h"
#include "Character/PlayerCharacter.h"
#include "Components/SphereComponent.h" //
#include "Components/StaticMeshComponent.h"

AKeyItemBase::AKeyItemBase()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;

	// 2. 콜리전 설정을 C++에서 직접 해줍니다.
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 물리적 충돌은 없고, 쿼리(LineTrace 등)에만 반응하도록 설정
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 기본적으로 모든 채널에 반응하지 않도록 설정
	Sphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // 오직 'Visibility' 채널의 LineTrace에만 반응(Block)하도록 설정
	Sphere->SetSphereRadius(50.f); // 구체의 반지름 설정 (블루프린트에서 수정 가능)

	// 3. 스태틱 메시(ItemMesh)를 생성하고, 루트인 SphereCollider에 부착합니다.
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);
	// 4. 스태틱 메시 자체의 콜리전은 꺼버립니다. (이제 SphereCollider가 콜리전을 담당하므로)
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKeyItemBase::Interact_Implementation(APlayerCharacter* Interactor)
{
	if (Interactor)
	{
		Interactor->PickupItem(ItemType);
	}
	
	Destroy();
}

EInteractState AKeyItemBase::GetInteractableType_Implementation()
{
	return EInteractState::Pickup;
}



