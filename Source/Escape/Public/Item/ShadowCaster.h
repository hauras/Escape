
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "ShadowCaster.generated.h"
class USphereComponent;
class USpotLightComponent;
class ARevealableDecalActor;

UCLASS()
class ESCAPE_API AShadowCaster : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AShadowCaster();
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
	virtual EInteractState GetInteractableType_Implementation() override;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USpotLightComponent> SpotLight;

	// 빛이 닿는 최대 거리
	UPROPERTY(EditAnywhere, Category = "Light Settings")
	float TraceDistance = 1500.f;

private:

};
