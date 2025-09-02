#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"

void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController * PlayerController = World->GetFirstPlayerController();
		if (PlayerController && SlashOverlayClassToSpawn)
		{
			SlashOverlay = CreateWidget<USlashOverlay>(PlayerController, SlashOverlayClassToSpawn);
			if (SlashOverlay)
			{
				SlashOverlay->AddToViewport();
			}
		}
	}
}
