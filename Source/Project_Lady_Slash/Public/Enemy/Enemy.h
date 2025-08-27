#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter/BaseCharacter.h"
#include "Characters/CharacterTypesEnums.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	bool IsEnemyDead();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	bool IsEnemyEngaged();
	bool IsEnemyAttacking();
	bool IsCharacterInsideAttackRadius();
	bool IsEnemyChasing();
	bool IsCharacterOutsideAttackRadius();
	void EnemyChaseTarget();
	bool IsCharacterOutsideCombatRadius();
	void EnemyStartPatrolling();
	void LoseInterest();
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	virtual void HandleDamageBaseCharacter(float DamageAmount) override;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> EnemyDeathPose;
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	bool InTargetRange(AActor* Target, double AcceptanceRaduis);
	void MoveToTarget(AActor* Target);
	AActor* SelectPatrolTarget();
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
	void ClearPatrolTimer();
	void ClearAttackTimer();
	virtual void Attack() override;
	virtual bool CanBaseCharacterAttack() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	/*Play Montage Sections*/
	virtual int32 PlayDeathMontage() override;
	/*Play Montage Sections*/

	/*HealthBar Component*/
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* EnemyHealthBar;
	void HideHealthBar();
	void ShowHealthBar();
	/*HealthBar Component*/

	/*EnemyAI Navigation / Behavior */
	UPROPERTY(EditAnywhere, Category = "Enemy AI Navigation")
	float PatrollingSpeed = 125.f;
	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	float ChasingSpeed = 300.f;
	UPROPERTY(EditInstanceOnly, Category = "EnemyAI Navigation / Behavior")
	AActor* CurrentPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "EnemyAI Navigation / Behavior")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(VisibleAnywhere, Category = "EnemyAI Navigation / Behavior")
	class UPawnSensingComponent* EnemySensing;
	UPROPERTY()
	AActor* CombatTarget;
	UPROPERTY()
	class AAIController* EnemyAIController;
	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	float WaitTimeMin = 3.f;
	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	float WaitTimeMax = 8.f;
	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	double CombatRadius = 500.f;
	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	double AttackRadius = 150.f;
	UPROPERTY(EditAnywhere)
	double PatrolRaduis = 200.f;

	FTimerHandle EnemyAttackTimer;
	void StartEnemyAttackTimer();
	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	float EnemyAttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	float EnemyAttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	float DeathLifeSpan = 8.f;
	/*EnemyAI Navigation / Behavior*/
};
