#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter/BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterTypesEnums.h"
#include "SlashCharacter.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	/** <IHitInterface*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface*/
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:

	virtual void BeginPlay() override;
	/*Callbacks for Input*/
	void EchoMove(const FInputActionValue& Value);
	void EchoLook(const FInputActionValue& Value);
	void EKeyPressed();
	void EchoArmTheWeapon();
	void EchoDisarmTheWepaon();
	void EchoPlayEquipSound();
	void EchoEquippingTheWeapon(AWeapon* Weapon);
	virtual void Attack() override;
	void Dodge();
	/*Callbacks for Input*/

	/*Play Montage Sections*/
	virtual bool CanBaseCharacterAttack() override;
	bool CanEchoUnEquipWeapon();
	bool CanEchoEquipWeapon();
	void PlayEquipMontage(const FName& SectionName);
	/*Play Montage Sections*/

	/*Anim Notifies*/
	virtual void AttackEnd() override;
	UFUNCTION(BlueprintCallable)
	void AddToSpineSocket();
	UFUNCTION(BlueprintCallable)
	void AddToHandSocket();
	UFUNCTION(BlueprintCallable)
	void EquippingEnd();
	UFUNCTION(BlueprintCallable)
	void HitReactingEnd();
	/*Anim Notifies*/

	/*InputActions*/
	UPROPERTY(EditAnywhere, Category = "Echo Input")
	class UInputMappingContext* EchoMappingContext;
	UPROPERTY(EditAnywhere, Category = "Echo Input")
	class UInputAction* EchoMovementAction;
	UPROPERTY(EditAnywhere, Category = "Echo Input")
	UInputAction* EchoLookAction;
	UPROPERTY(EditAnywhere, Category = "Echo Input")
	UInputAction* EchoJumpAction;
	UPROPERTY(EditAnywhere, Category = "Echo Input")
	UInputAction* EchoEKeyAction;
	UPROPERTY(EditAnywhere, Category = "Echo Input")
	UInputAction* EchoAttackAction;
	UPROPERTY(EditAnywhere, Category = "Echo Input")
	UInputAction* EchoDodgeAction;
	/*InputActions*/

	/*Montages Variables*/
	UPROPERTY(EditDefaultsOnly, Category = "Echo Montages")
	UAnimMontage* EquipMontage;
	/*Montages Variables*/

private:

	void InitializeSlashOverlay(APlayerController* PlayerController);
	void InitializeEnhancedInput(APlayerController* PlayerController);
	bool IsEchoUnoccupied();
	void SetEchoHUDHealth();

	/*States*/
	ECharacterState EchoCurrentState = ECharacterState::ECS_UnEquipped;
	EActionState EchoActionState = EActionState::EAS_Unoccupied;
	/*States*/

	/*Echo Components*/
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* EchoSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* EchoCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	class UGroomComponent* EchoHair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* EchoEyebrows;
	/*Echo Components*/

	/*Pointers*/
	UPROPERTY(VisibleInstanceOnly)
	class AItem* OverlappingItem;

	UPROPERTY(EditAnywhere, Category = "Echo Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY()
	class USlashOverlay* SlashOverlay;
	/*Pointers*/

public: 
	/*Setters and Getters*/
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetEchoCharacterState() const { return this->EchoCurrentState; }
	/*Setters and Getters*/
};
