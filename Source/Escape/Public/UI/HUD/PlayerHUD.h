// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UEscapeUserWidget;
/**
 * 
 */
UCLASS()
class ESCAPE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEscapeUserWidget> EscapeUserWidgetClass;

	UPROPERTY()
	TObjectPtr<UEscapeUserWidget> EscapeUserWidget;
};
