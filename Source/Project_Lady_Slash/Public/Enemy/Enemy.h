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
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	void Die();
	UPROPERTY(BlueprintReadOnly)
	EDeathPose EnemyDeathPose = EDeathPose::EDP_Alive;
	bool InTargetRange(AActor* Target, double AcceptanceRaduis);
	void MoveToTarget(AActor* Target);
	AActor* SelectPatrolTarget();

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

	/*Enemy Effects*/
	UPROPERTY(EditAnywhere, Category = "Enemy Sounds")
	USoundBase* HitSounds;

	UPROPERTY(EditAnywhere, Category = "Enemy VFx")
	UParticleSystem* HitParticle;
	/*Enemy Effects*/

	/*Attributes Component*/
	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* EnemyAttributes;

	/*HealthBar Component*/
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* EnemyHealthBar;

	/*Enemy Navigation*/
	UPROPERTY(EditInstanceOnly, Category = "Enemy AI Navigation")
	AActor* CurrentPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "Enemy AI Navigation")
	TArray<AActor*> PatrolTargets;

	UPROPERTY()
	AActor* CombatTarget;

	UPROPERTY()
	class AAIController* EnemyAIController;

	FTimerHandle PatrolTimer;

	void PatrolTimerFinished();

	UPROPERTY(EditAnywhere, Category = "Enemy AI Navigation")
	float WaitTimeMin = 3.f;

	UPROPERTY(EditAnywhere, Category = "Enemy AI Navigation")
	float WaitTimeMax = 8.f;

	UPROPERTY(EditAnywhere)
	double CombatRaduis = 500.f;

	UPROPERTY(EditAnywhere)
	double PatrolRaduis = 200.f;
	/*Enemy Navigation*/
};
