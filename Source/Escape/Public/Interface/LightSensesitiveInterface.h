
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LightSensesitiveInterface.generated.h"

UINTERFACE(MinimalAPI)
class ULightSensesitiveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ESCAPE_API ILightSensesitiveInterface
{
	GENERATED_BODY()
public:

	// 빛을 받았을때 호출할 함수 
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Light Interaction")
	void OnLightReceived(float Intensity, float DeltaTime);
};
