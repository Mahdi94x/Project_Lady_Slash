#include "Items/HealthPickup/HealthPickup.h"
#include "Interfaces/PickUpInterface.h"


void AHealthPickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		PickUpInterface->HealthPickup(this);
		Destroy();
	}
}
