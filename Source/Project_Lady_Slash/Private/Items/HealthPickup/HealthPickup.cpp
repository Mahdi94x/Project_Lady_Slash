#include "Items/HealthPickup/HealthPickup.h"
#include "Interfaces/PickUpInterface.h"


void AHealthPickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		HealthFinalValue = SetHealthValue(HealthMinlValue, HealthMaxlValue);
		PickUpInterface->HealthPickup(this);
		SpawnPickupEffect();
		PlayPickupSound();
		Destroy();
	}
}

int32 AHealthPickup::SetHealthValue(int32 MinValue, int32 MaxValue)
{
	const int32 Value = FMath::RandRange(MinValue, MaxValue);
	return Value;
}
