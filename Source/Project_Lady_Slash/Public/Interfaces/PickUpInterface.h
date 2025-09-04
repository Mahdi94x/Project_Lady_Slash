#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT_LADY_SLASH_API IPickUpInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetOverlappingItem(class AItem* Item);
	virtual void SoulsPickup(class ASoulsPickup* Soul);
	virtual void HealthPickup(class AHealthPickup* Health);
	virtual void TreasurePickup(class ATreasure* Treasure);

};
