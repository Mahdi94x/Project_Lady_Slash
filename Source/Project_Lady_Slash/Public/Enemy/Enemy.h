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
	/** <AActor> */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface*/

protected:
	/** <AActor> */
	virtual void BeginPlay() override;
	/** <AActor> */

	/** <ABaseCharacter> */
	virtual void Die_Implementation() override;
	virtual void HandleDamageBaseCharacter(float DamageAmount) override;
	virtual void Attack() override;
	virtual bool CanBaseCharacterAttack() override;
	virtual void AttackEnd() override;
	/** </ABaseCharacter> */
	
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	

private:
	/** AI Behavior */
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	bool IsEnemyDead();
	bool IsEnemyEngaged();
	bool IsEnemyAttacking();
	bool IsCharacterInsideAttackRadius();
	bool IsEnemyChasing();
	bool IsCharacterOutsideAttackRadius();
	void EnemyChaseTarget();
	bool IsCharacterOutsideCombatRadius();
	void EnemyStartPatrolling();
	void LoseInterest();
	void HideHealthBar();
	void ShowHealthBar();
	void ClearPatrolTimer();
	void ClearAttackTimer();
	void StartEnemyAttackTimer();
	bool InTargetRange(AActor* Target, double AcceptanceRaduis);
	void MoveToTarget(AActor* Target);
	AActor* SelectPatrolTarget();
	void SpawnDefaultWeapon();
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn); // CallBack For OnPawnSeen in UPawnSensingComponent
	void EnemyDeadDrop();

	FTimerHandle PatrolTimer;
	FTimerHandle EnemyAttackTimer;
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* EnemyHealthBar;
	UPROPERTY(VisibleAnywhere, Category = "EnemyAI Behavior")
	class UPawnSensingComponent* EnemySensing;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;
	
	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	double CombatRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	double AttackRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	double AcceptanceRadius = 50.f;

	UPROPERTY(EditInstanceOnly, Category = "EnemyAI Behavior")
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "EnemyAI Behavior")
	TArray<AActor*> PatrolTargets;

	UPROPERTY()
	class AAIController* EnemyAIController;

	UPROPERTY(EditAnywhere)
	double PatrolRaduis = 200.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	float WaitTimeMin = 3.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	float WaitTimeMax = 8.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	float EnemyAttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	float EnemyAttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	float DeathLifeSpan = 8.f;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	TArray<TSubclassOf<class AItem>> EnemyDropItemsArray;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	int32 EnemySoulMinDrop;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	int32 EnemySoulMaxDrop;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	int32 EnemyHealthMinDrop;

	UPROPERTY(EditAnywhere, Category = "EnemyAI Behavior")
	int32 EnemyHealthMaxDrop;
};
