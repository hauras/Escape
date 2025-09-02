
#pragma once

#include "CoreMinimal.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "DoorBase.generated.h"

class UStaticMeshComponent;

UCLASS()
class ESCAPE_API ADoorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ADoorBase();

	virtual void Interact_Implementation(APlayerCharacter* Interactor) override;
	virtual EInteractState GetInteractableType_Implementation() override; // 타입을 EInteractState로 변경

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> DoorFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> Door;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door State")
	bool bIsLocked = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door State")
	bool bIsOpen = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door State")
	EItemType RequiredItemType = EItemType::EItemType_Key;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
	void OpenDoor();


};
