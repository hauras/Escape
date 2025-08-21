
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "ShadowCaster.generated.h"
class USphereComponent;

UCLASS()
class ESCAPE_API AShadowCaster : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AShadowCaster();

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
	virtual EInteractState GetInteractableType_Implementation() override;
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> Sphere;
};
