

#include "Character/EnemyCharacter.h"

#include "AIController/AIController_Guardian.h"
#include "BehaviorTree/BlackboardComponent.h"

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                  class AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!bIsStunned)
	{
		Stun();
	}
	return DamageAmount;
}

void AEnemyCharacter::Stun()
{
	bIsStunned = true;

	if (AAIController_Guardian* AIC = Cast<AAIController_Guardian>(GetController()))
	{
		AIC->GetBlackboardComponent()->SetValueAsBool(FName("bIsStunned"), true);
	}
	GetWorld()->GetTimerManager().SetTimer(StunTimer, this, &AEnemyCharacter::EndStun, StunDuration, false);

	UE_LOG(LogTemp, Warning, TEXT("Guardian STUNNED!"));

}

void AEnemyCharacter::EndStun()
{
	bIsStunned = false;

	if (AAIController_Guardian* AIC = Cast<AAIController_Guardian>(GetController()))
	{
		AIC->GetBlackboardComponent()->SetValueAsBool(FName("bIsStunned"), false);
	}
	UE_LOG(LogTemp, Warning, TEXT("Guardian stun ENDED."));

}
