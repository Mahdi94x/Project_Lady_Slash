#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void SetWeaponBoxCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	class AWeapon* EquippedWeapon;
	virtual void Attack();
	virtual void Die();
	virtual void PlayAttackMontage();
	virtual bool CanBaseCharacterAttack();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* CharacterAttributes;
	/*Montages Variables*/
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* HitReactMontage;
	/*Montages Variables*/
	bool IsBaseCharacterAlive();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamageBaseCharacter(float DamageAmount);

private:
	/*BaseCharacter Effects*/
	UPROPERTY(EditAnywhere, Category = "Character SFx")
	USoundBase* HitSounds;
	UPROPERTY(EditAnywhere, Category = "Character VFx")
	UParticleSystem* HitParticle;
	/*BaseCharacter Effects*/

};
