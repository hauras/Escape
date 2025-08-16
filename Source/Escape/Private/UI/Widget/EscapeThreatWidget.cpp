

#include "UI/Widget/EscapeThreatWidget.h"
#include "Components/ProgressBar.h" 
#include "GameState/EscapeGameState.h"
#include "Kismet/GameplayStatics.h"

void UEscapeThreatWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UWidget* FoundWidget = GetWidgetFromName(TEXT("ProgressBar_Threat"));
	if (IsValid(FoundWidget))
	{
		// 찾았다면, UProgressBar 타입으로 캐스팅하여 우리 변수에 저장합니다.
		ProgressBar_Threat = Cast<UProgressBar>(FoundWidget);
	}
    
	if (!IsValid(ProgressBar_Threat))
	{
		// 만약 못 찾았거나 캐스팅에 실패했다면, 로그를 남겨서 문제를 알려줍니다.
		UE_LOG(LogTemp, Error, TEXT("Could not find or cast ProgressBar_Threat in WBP_Threat!"));
	}
	// ---------------------------------------------

	// GameState를 찾아 델리게이트 구독하는 로직은 동일합니다.
	GameState = Cast<AEscapeGameState>(UGameplayStatics::GetGameState(this));
	if (IsValid(GameState))
	{
		GameState->OnThreatChanged.AddDynamic(this, &UEscapeThreatWidget::OnThreatChanged);
		OnThreatChanged(GameState->GetCurrentThreat(), GameState->GetMaxThreat());
	}
}

void UEscapeThreatWidget::OnThreatChanged(float CurrentThreat, float MaxThreat)
{
	if (MaxThreat > 0.f)
	{
		CurrentPercent = CurrentThreat / MaxThreat;
	}

	// ProgressBar가 유효하다면, C++에서 직접 퍼센트 설정
	if (ProgressBar_Threat)
	{
		ProgressBar_Threat->SetPercent(CurrentPercent);
	}

	// 계산된 퍼센트를 기준으로, 블루프린트에 신호 전송
	if (CurrentPercent >= 1.f)
	{
		OnThreatIsFull();
	}
	else
	{
		OnThreatIsNotFull();
	}
}
