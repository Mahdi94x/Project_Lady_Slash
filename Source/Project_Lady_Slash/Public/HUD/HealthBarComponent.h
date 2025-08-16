#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	void SetHealthBarPercent(float Percent);

private:
	UPROPERTY()
	class UHealthBarWidget* HealthBarWidget;
};
