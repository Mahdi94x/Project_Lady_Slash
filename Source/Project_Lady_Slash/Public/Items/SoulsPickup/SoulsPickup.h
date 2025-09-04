#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "SoulsPickup.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API ASoulsPickup : public AItem
{
	GENERATED_BODY()
	
protected:
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
