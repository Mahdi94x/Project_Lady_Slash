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

private: 
	UPROPERTY(VisibleAnywhere, Category = "Soul Properties")
	int32 SoulFinalValue;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 SoulMinlValue;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 SoulMaxlValue;

public:
	FORCEINLINE int32 SetSoulValue(int32 MinValue, int32 MaxValue);
	FORCEINLINE int32 GetSoulFinalValue() const { return this->SoulFinalValue; }
};
