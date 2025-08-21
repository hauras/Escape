
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

UENUM(BlueprintType)
enum class EInteractState : uint8
{
	Pickup, // 줍기
	Rotate, // 회전시키기
	Open, // 열기
};
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ESCAPE_API IInteractInterface
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(APlayerCharacter* Interactor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	EInteractState GetInteractableType();
};
