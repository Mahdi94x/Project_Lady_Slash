#include "Items/Treasure/Treasure.h"
#include "Interfaces/PickUpInterface.h"


void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->TreasurePickup(this);
		SpawnPickupEffect();
		PlayPickupSound();
		Destroy();
	}
}
