

#include "Item/RevealableDecalActor.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ARevealableDecalActor::ARevealableDecalActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ARevealableDecalActor::BeginPlay()
{
	Super::BeginPlay();

	if (TargetActor)
	{
		TargetActor->SetActorHiddenInGame(true);
		TargetActor->SetActorEnableCollision(false);
	}
}

void ARevealableDecalActor::Reveal()
{
	if (TargetActor)
	{
		TargetActor->SetActorHiddenInGame(false);
		TargetActor->SetActorEnableCollision(true);
	}
}

void ARevealableDecalActor::Hide()
{
	if (TargetActor)
	{
		TargetActor->SetActorHiddenInGame(true);
		TargetActor->SetActorEnableCollision(false);
	}
}


