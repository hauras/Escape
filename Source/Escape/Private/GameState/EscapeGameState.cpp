

#include "GameState/EscapeGameState.h"

void AEscapeGameState::AddThreat(float Amount)
{
	const float OldThreat = CurrentThreat;

	CurrentThreat = FMath::Clamp(CurrentThreat + Amount, 0.f, MaxThreat);
    
	// 값이 실제로 변경되었을 때만 방송(Broadcast)합니다.
	if (OldThreat != CurrentThreat)
	{
		// 변경된 현재값과 최대값을 함께 방송합니다.
		OnThreatChanged.Broadcast(CurrentThreat, MaxThreat);
	}
}
