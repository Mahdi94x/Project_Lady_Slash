#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypesEnums.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;
	/** <IHitInterface*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface*/
	virtual void Attack();

	UFUNCTION(BlueprintNativeEvent)
	void Die();
	virtual bool CanBaseCharacterAttack();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);
	bool IsBaseCharacterAlive();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	virtual void HandleDamageBaseCharacter(float DamageAmount);
	virtual int32 PlayDeathMontage();
	virtual int32 PlayAttackMontage();
	void DisableCapsule();
	void StopAttackMontage();
	void DisableMeshCollision();

	UFUNCTION(BlueprintCallable)
	void SetWeaponBoxCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* CharacterAttributes;
	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	class AWeapon* EquippedWeapon;
	UPROPERTY(BlueprintReadOnly)
	AActor* CombatTarget;
	UPROPERTY(EditAnywhere, Category = "EnemyAI Navigation / Behavior")
	double WarpTargetDistance = 75.f;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

private:
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	/*BaseCharacter Effects*/
	UPROPERTY(EditAnywhere, Category = "Character SFx")
	USoundBase* HitSounds;
	UPROPERTY(EditAnywhere, Category = "Character VFx")
	UParticleSystem* HitParticle;
	/*BaseCharacter Effects*/

	/*Montages Variables*/
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* DeathMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* HitReactMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	TArray<FName> AttackMontageSections;
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	TArray<FName> DeathMontageSections;
	/*Montages Variables*/

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return this->DeathPose; }
};
