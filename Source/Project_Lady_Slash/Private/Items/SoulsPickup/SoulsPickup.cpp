#include "Items/SoulsPickup/SoulsPickup.h"
#include "Interfaces/PickUpInterface.h"


void ASoulsPickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->SoulsPickup(this);
		Destroy();
	}
}
