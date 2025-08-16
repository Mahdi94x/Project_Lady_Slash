#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBarWidget.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthBarPercent(float Percent)
{
	if (HealthBarWidget == nullptr)
	{
		HealthBarWidget = Cast<UHealthBarWidget>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthProgressBar)
	{
		HealthBarWidget->HealthProgressBar->SetPercent(Percent);
	}
}
