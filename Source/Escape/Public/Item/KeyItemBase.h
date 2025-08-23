
#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "KeyItemBase.generated.h"

class USphereComponent;

UCLASS()
class ESCAPE_API AKeyItemBase : public AActor, public IInteractInterface 
{
	GENERATED_BODY()
	
public:	
	AKeyItemBase();

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
	virtual EInteractState GetInteractableType_Implementation() override;

	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EItemType ItemType = EItemType::EItemType_None;
};
