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

void UAttributeComponent::UpdateCoinsAttribute(int32 TreasureValue)
{
	this->CoinsAttribute += TreasureValue;
}

void UAttributeComponent::UpdateSoulsAttribute(int32 SoulsValue)
{
	this->SoulsAttribute += SoulsValue;
}

