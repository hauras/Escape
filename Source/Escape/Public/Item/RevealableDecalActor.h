
#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "RevealableDecalActor.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API ARevealableDecalActor : public ADecalActor
{
	GENERATED_BODY()
public:
	ARevealableDecalActor();

	void Reveal();
	void Hide();

protected:
	virtual void BeginPlay() override;
private:

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

	UPROPERTY(EditAnywhere, Category = "Decal Settings")
	TObjectPtr<AActor> TargetActor;

};
