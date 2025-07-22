#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleInstanceOnly, Category = "Item Periodic Behavior")
	double RunningTime = 0;

	UPROPERTY(EditAnywhere, Category = "Item Periodic Behavior")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Item Periodic Behavior")
	float TimeConstant = 5.f;
};
