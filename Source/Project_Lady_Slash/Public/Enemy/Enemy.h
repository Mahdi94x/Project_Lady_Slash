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

	virtual void GetHit(const FVector& ImpactPoint) override;

protected:

	virtual void BeginPlay() override;

	/*Montages Variables*/
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Montages")
	UAnimMontage* HitReactMontage;
	/*Montages Variables*/

	/*Play Montage Sections*/
	void PlayHitReactMontage(const FName& SectionName);
	/*Play Montage Sections*/
};
