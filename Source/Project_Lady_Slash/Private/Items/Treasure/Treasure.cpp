#include "Items/Treasure/Treasure.h"
#include "Interfaces/PickUpInterface.h"
#include "Kismet/GameplayStatics.h"


void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
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
