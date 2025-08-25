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
	void CheckPatrolTarget();
	void CheckCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
protected:
	virtual void BeginPlay() override;
	virtual void Die() override;
	UPROPERTY(BlueprintReadOnly)
	EDeathPose EnemyDeathPose = EDeathPose::EDP_Alive;
	bool InTargetRange(AActor* Target, double AcceptanceRaduis);
	void MoveToTarget(AActor* Target);
	AActor* SelectPatrolTarget();
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	/*Play Montage Sections*/
	void PlayDeathMontage();
	/*Play Montage Sections*/

	/*HealthBar Component*/
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* EnemyHealthBar;

	/*Enemy Navigation*/
	UPROPERTY(EditInstanceOnly, Category = "Enemy AI Navigation")
	AActor* CurrentPatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "Enemy AI Navigation")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(VisibleAnywhere, Category = "Enemy AI Navigation")
	class UPawnSensingComponent* EnemySensing;
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
	UPROPERTY(EditAnywhere, Category = "Enemy AI Navigation")
	double CombatRadius = 500.f;
	UPROPERTY(EditAnywhere, Category = "Enemy AI Navigation")
	double AttackRadius = 150.f;
	UPROPERTY(EditAnywhere)
	double PatrolRaduis = 200.f;
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	/*Enemy Navigation*/
};
