#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetEchoHealthProgressBarPercent(float Percent);
	void SetEchoStaminaProgressBarPercent(float Percent);
	void SetCoinsCountText(int32 Coins);
	void SetSoulsCountText(int32 Souls);
private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* EchoHealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* EchoStaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CoinsCountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulsCountText;

};
