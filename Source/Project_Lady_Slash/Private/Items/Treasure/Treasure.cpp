#include "Items/Treasure/Treasure.h"
#include "Characters/SlashCharacter.h"
#include "Kismet/GameplayStatics.h"


void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		if (PickupSounds)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSounds,
				GetActorLocation()
			);
		}

		Destroy();
	}
}
