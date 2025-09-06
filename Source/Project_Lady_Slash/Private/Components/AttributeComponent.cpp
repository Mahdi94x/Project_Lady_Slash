#include "Components/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateCharacterAttributes();
}

bool UAttributeComponent::IsCharacterAlive()
{
	return CurrentHealth > 0.f;
}

void UAttributeComponent::DecreaseCurrentHealth(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::IncreaseCurrentHealth(float HealthPickupValue)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthPickupValue, 0.f, MaxHealth);
}

float UAttributeComponent::GetCurrentHealthPercent()
{

	return CurrentHealth / MaxHealth;
}

void UAttributeComponent::HealthRegeneration(float DeltaTime)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + (DeltaTime * HealthRegenRate), 0.f, MaxHealth);
}

void UAttributeComponent::UpdateCoinsAttribute(int32 TreasureValue)
{
	this->CoinsAttribute += TreasureValue;
}

void UAttributeComponent::UpdateSoulsAttribute(int32 SoulsValue)
{
	this->SoulsAttribute += SoulsValue;
}

float UAttributeComponent::GetCurrentStaminaPercent()
{
	return CurrentStamina / MaxStamina;
}

void UAttributeComponent::DecreaseCurrentStamina(float Cost)
{
	CurrentStamina = FMath::Clamp(CurrentStamina - Cost, 0.f, MaxStamina);
}

void UAttributeComponent::StaminaRegeneration(float DeltaTime)
{
	CurrentStamina = FMath::Clamp(CurrentStamina + (DeltaTime * StaminaRegenRate), 0.f, MaxStamina);
}

void UAttributeComponent::UpdateCharacterAttributes()
{
	if (this->SoulsAttribute >= SoulsToNextUpgrade)
	{
		this->SoulsAttribute = 0.f;
		this->CharacterLevel += 1.f;
		this->SoulsToNextUpgrade += 1.f;
		this->MaxHealth += 10.f;
		this->CurrentHealth = MaxHealth;
		this->MaxStamina += 10.f;
		this->DodgeCost -= 0.5f;
		this->CurrentStamina = MaxStamina;
	}
}