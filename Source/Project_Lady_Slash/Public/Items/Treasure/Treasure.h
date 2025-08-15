#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasure.generated.h"


UCLASS()
class PROJECT_LADY_SLASH_API ATreasure : public AItem
{
	GENERATED_BODY()
	
protected:
	
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:

	UPROPERTY(EditAnywhere, Category = "Treasure Sounds")
	USoundBase* PickupSounds;

	UPROPERTY(EditAnywhere, Category = "Treasure Properties")
	int32 GoldValue;
};
