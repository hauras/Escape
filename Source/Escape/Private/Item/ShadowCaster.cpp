

#include "Item/ShadowCaster.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h" // 헤더 추가
#include "Item/RevealableDecalActor.h"   // 헤더 추가 (경로 확인)
#include "Kismet/KismetSystemLibrary.h"   // 디버깅용 헤더 (선택 사항)
#include "DrawDebugHelpers.h"
#include "Interface/LightSensesitiveInterface.h"

AShadowCaster::AShadowCaster()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 물리적 충돌은 없고, 쿼리(LineTrace 등)에만 반응하도록 설정
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 기본적으로 모든 채널에 반응하지 않도록 설정
	Sphere->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // 오직 'Visibility' 채널의 LineTrace에만 반응(Block)하도록 설정
	Sphere->SetSphereRadius(50.f);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh -> SetupAttachment(RootComponent);
	StaticMesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	SpotLight->SetupAttachment(RootComponent);
}

void AShadowCaster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// SpotLight 포인터 유효성 검사 (안전장치)
	if (!SpotLight)
	{
		return;
	}

	FVector StartLocation = SpotLight->GetComponentLocation();
	FVector EndLocation = StartLocation + (SpotLight->GetForwardVector() * TraceDistance);
	FHitResult HitResult;

	// --- 디버그 코드 1: 라인 트레이스를 노란색 선으로 그립니다 ---
	// 이 선이 실제로 어디로 향하는지, 어디에 부딪히는지 눈으로 확인할 수 있습니다.
	DrawDebugLine(
		GetWorld(),
		StartLocation,
		EndLocation,
		FColor::Yellow, // 라인 색상
		false,          // 계속 유지하지 않음
		-1,             // 프레임이 지속되는 동안
		0,              // 뎁스 우선순위
		2.0f            // 라인 두께
	);

	// 라인 트레이스 실행
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility
	);

	ARevealableDecalActor* CurrentHitDecal = nullptr;
	if (bHit)
	{
		// 부딪힌 액터를 우리가 만든 데칼 액터 클래스로 캐스팅
		CurrentHitDecal = Cast<ARevealableDecalActor>(HitResult.GetActor());

		// --- 디버그 코드 2: 라인이 부딪힌 지점에 구체를 그립니다 ---
		// 구체의 색상으로 캐스팅(형변환) 성공 여부를 알려줍니다.
		// 초록색 구체: 성공! ARevealableDecalActor를 정확히 맞췄습니다.
		// 빨간색 구체: 실패! 데칼이 아닌 다른 것(벽, 바닥 등)을 맞췄습니다.
		DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint, // 부딪힌 바로 그 지점
			25.0f,                 // 구체 크기
			12,                    // 구체 디테일
			CurrentHitDecal ? FColor::Green : FColor::Red, // 조건부 색상
			false,
			-1,
			0,
			1.f
		);
	}
	
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


