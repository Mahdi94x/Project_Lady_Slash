#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypesEnums.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API AEnemy : public ACharacter , public IHitInterface
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:

	virtual void BeginPlay() override;

	void Die();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose EnemyDeathPose = EDeathPose::EDP_Alive;

private:

	/*Montages Variables*/
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy Montages")
	UAnimMontage* DeathMontage;
	/*Montages Variables*/

	/*Play Montage Sections*/
	void PlayHitReactMontage(const FName& SectionName);

	void PlayDeathMontage();
	/*Play Montage Sections*/

	UPROPERTY(EditAnywhere, Category = "Enemy Sounds")
	USoundBase* HitSounds;

	UPROPERTY(EditAnywhere, Category = "Enemy VFx")
	UParticleSystem* HitParticle;

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* EnemyAttributes;

	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* EnemyHealthBar;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRaduis = 500.f;
	
};
