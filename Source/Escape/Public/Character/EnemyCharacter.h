
#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterBase.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPE_API AEnemyCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Stun();
	void EndStun();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsStunned = false;
	UPROPERTY(EditAnywhere, Category = "UI")
	float StunDuration = 3.f;

	FTimerHandle StunTimer;
};
