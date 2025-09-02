
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "Interface/LightSensesitiveInterface.h"
#include "LightChestBase.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ESCAPE_API ALightChestBase : public AActor, public IInteractInterface, public ILightSensesitiveInterface
{
	GENERATED_BODY()
	
public:	
	ALightChestBase();

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
	virtual EInteractState GetInteractableType_Implementation() override;
	virtual void OnLightReceived_Implementation(float Intensity, float DeltaTime) override;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ChestMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chest")
	float CurrentCharge = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chest")
	float MaxCharge = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chest")
	bool bIsUnLocked = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Chest")
	void OnChargeUpdated(float Percent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Chest")
	void OnUnlocked();
};
