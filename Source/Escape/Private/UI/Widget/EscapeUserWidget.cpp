

#include "UI/Widget/EscapeUserWidget.h"

#include "Character/PlayerCharacter.h"
#include "Controller/EscapeController.h"

void UEscapeUserWidget::SetOwningActor(AActor* NewOwner)
{
	if (NewOwner)
	{
		OwningPlayerCharacter = Cast<APlayerCharacter>(NewOwner);
		if (OwningPlayerCharacter)
		{
			OwningPlayerController = Cast<AEscapeController>(OwningPlayerCharacter->GetController());
		}
	}
}

void UEscapeUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!OwningPlayerCharacter)
	{
		SetOwningActor(GetOwningPlayerPawn());
	}
}
