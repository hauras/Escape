

#include "UI/HUD/PlayerHUD.h"

#include "UI/Widget/EscapeUserWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (EscapeUserWidgetClass)
	{
		EscapeUserWidget = CreateWidget<UEscapeUserWidget>(GetWorld(), EscapeUserWidgetClass);
		EscapeUserWidget->AddToViewport();
	}
}
