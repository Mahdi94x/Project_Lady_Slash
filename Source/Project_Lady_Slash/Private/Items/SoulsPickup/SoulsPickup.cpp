#include "Items/SoulsPickup/SoulsPickup.h"
#include "Interfaces/PickUpInterface.h"


void ASoulsPickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* PickUpInterface = Cast<IPickUpInterface>(OtherActor);
	if (PickUpInterface)
	{
		SoulFinalValue = SetSoulValue(SoulMinlValue, SoulMaxlValue);
		PickUpInterface->SoulsPickup(this);
		SpawnPickupEffect();
		PlayPickupSound();
		Destroy();
	}
}

int32 ASoulsPickup::SetSoulValue(int32 MinValue, int32 MaxValue)
{
	const int32 Value = FMath::RandRange(MinValue, MaxValue);
	return Value;
}
