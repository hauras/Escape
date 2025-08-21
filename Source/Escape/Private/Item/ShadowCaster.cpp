

#include "Item/ShadowCaster.h"

#include "Components/SphereComponent.h"

AShadowCaster::AShadowCaster()
{

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 물리적 충돌은 없고, 쿼리(LineTrace 등)에만 반응하도록 설정
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 기본적으로 모든 채널에 반응하지 않도록 설정
	Sphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // 오직 'Visibility' 채널의 LineTrace에만 반응(Block)하도록 설정
	Sphere->SetSphereRadius(50.f);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh -> SetupAttachment(RootComponent);
	StaticMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AShadowCaster::Interact_Implementation(APlayerCharacter* Interactor)
{
	const FRotator RotationToAdd = FRotator(0.f, 90.f, 0.f);

	AddActorLocalRotation(RotationToAdd);
}

EInteractState AShadowCaster::GetInteractableType_Implementation()
{
	return EInteractState::Rotate;
}


