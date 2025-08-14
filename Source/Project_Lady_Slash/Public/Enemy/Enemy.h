#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
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

protected:

	virtual void BeginPlay() override;

	

private:

	/*Montages Variables*/
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Montages")
	UAnimMontage* HitReactMontage;
	/*Montages Variables*/

	/*Play Montage Sections*/
	void PlayHitReactMontage(const FName& SectionName);
	/*Play Montage Sections*/

	UPROPERTY(EditAnywhere, Category = "Enemy Sounds")
	USoundBase* HitSounds;

	UPROPERTY(EditAnywhere, Category = "Enemy VFx")
	UParticleSystem* HitParticle;
};
