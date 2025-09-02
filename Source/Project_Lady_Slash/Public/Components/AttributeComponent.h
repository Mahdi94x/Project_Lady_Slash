#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_LADY_SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

private:
	/*Character Attributes Variables*/
	UPROPERTY(EditAnywhere, Category = "Character Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Character Attributes")
	float MaxHealth;
	/*Character Attributes Variables*/

public: /*Setters - Getters - Updaters*/

	FORCEINLINE void UpdateCurrentHealth(float Damage);
	FORCEINLINE float GetCurrentHealthPercent();
	bool IsCharacterAlive();
};
