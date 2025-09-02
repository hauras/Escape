

#include "Item/LightActivatedObject.h"

ALightActivatedObject::ALightActivatedObject()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ALightActivatedObject::OnLightReceived_Implementation(float Intensity, float DeltaTime)
{
	if (!bHasActivated)
	{
		bHasActivated = true;
		ActivateStairs();

		UE_LOG(LogTemp, Warning, TEXT("ActivateStairs"));
	}
}





