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
	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	int32 CharacterLevel = 1;

	UPROPERTY(EditAnywhere, Category = "Character Attributes")
	float CurrentHealth;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	float MaxHealth = 50;
	
	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	float HealthRegenRate = 2.f;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	int32 CoinsAttribute;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	int32 SoulsAttribute;

	UPROPERTY(EditAnywhere, Category = "Character Attributes")
	float CurrentStamina;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	float MaxStamina = 60;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	float DodgeCost = 10.f;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	float StaminaRegenRate = 2.f;

	UPROPERTY(VisibleAnywhere, Category = "Character Attributes")
	float SoulsToNextUpgrade = 5.f;

	/*Character Attributes Variables*/

public: /*Setters - Getters - Updaters*/

	void DecreaseCurrentHealth(float Damage);
	void IncreaseCurrentHealth(float HealthPickupValue);
	float GetCurrentHealthPercent();
	FORCEINLINE int32 GetCoinsAttribute() const { return this->CoinsAttribute; }
	FORCEINLINE int32 GetSoulsAttribute() const { return this->SoulsAttribute; }
	void UpdateCoinsAttribute(int32 TreasureValue);
	void UpdateSoulsAttribute(int32 SoulsValue);
	float GetCurrentStaminaPercent();
	void DecreaseCurrentStamina(float Cost);
	float GetCurrentStamina() const { return this->CurrentStamina; }
	void StaminaRegeneration(float DeltaTime);
	void HealthRegeneration(float DeltaTime);
	float GetDodgeCost() const { return this->DodgeCost; }
	void UpdateCharacterAttributes();

};
