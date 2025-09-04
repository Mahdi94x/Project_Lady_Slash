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
	bool IsCharacterAlive();

protected:

	virtual void BeginPlay() override;

private:
	/*Character Attributes Variables*/
	UPROPERTY(EditAnywhere, Category = "Character Attributes")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Character Attributes")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	int32 CoinsAttribute;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	int32 SoulsAttribute;

	/*Character Attributes Variables*/

public: /*Setters - Getters - Updaters*/

	void DecreaseCurrentHealth(float Damage);
	void IncreaseCurrentHealth(float HealthPickupValue);
	float GetCurrentHealthPercent();
	FORCEINLINE int32 GetCoinsAttribute() const { return this->CoinsAttribute; }
	FORCEINLINE int32 GetSoulsAttribute() const { return this->SoulsAttribute; }
	void UpdateCoinsAttribute(int32 TreasureValue);
	void UpdateSoulsAttribute(int32 SoulsValue);

};
