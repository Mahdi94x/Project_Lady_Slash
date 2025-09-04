#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "HealthPickup.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API AHealthPickup : public AItem
{
	GENERATED_BODY()
	
protected:
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Health Properties")
	int32 HealthFinalValue;

	UPROPERTY(EditAnywhere, Category = "Health Properties")
	int32 HealthMinlValue;

	UPROPERTY(EditAnywhere, Category = "Health Properties")
	int32 HealthMaxlValue;

public:
	FORCEINLINE int32 SetHealthValue(int32 MinValue, int32 MaxValue);
	FORCEINLINE int32 GetHealthFinalValue() const { return this->HealthFinalValue; }
};
