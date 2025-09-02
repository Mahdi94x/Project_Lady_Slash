#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API ASlashHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Slash")
	TSubclassOf<class USlashOverlay> SlashOverlayClassToSpawn;

	UPROPERTY()
	USlashOverlay* SlashOverlay;
	
};
