#include "HUD/SlashOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void USlashOverlay::SetEchoHealthProgressBarPercent(float Percent)
{
	if (EchoHealthProgressBar)
	{
		EchoHealthProgressBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetEchoStaminaProgressBarPercent(float Percent)
{
	if (EchoStaminaProgressBar)
	{
		EchoStaminaProgressBar->SetPercent(Percent);
	}
}

void USlashOverlay::SetCoinsCountText(int32 Coins)
{
	if (CoinsCountText)
	{
		/*SetText require a text and the input parameter is an int32 so we convert from
		int32 -> FString using Printf and then convert from 
		FString -> FText using FromString*/
		CoinsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Coins)));
	}
}

void USlashOverlay::SetSoulsCountText(int32 Souls)
{
	if (SoulsCountText)
	{
		SoulsCountText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Souls)));
	}
}
