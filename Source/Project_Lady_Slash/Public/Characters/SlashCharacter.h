#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASlashCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

protected:

	virtual void BeginPlay() override;

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
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Echo Montages")
	UAnimMontage* EquipMontage;
	/*Montages Variables*/

	/*Callbacks for Input*/
	void EchoMove(const FInputActionValue& Value);
	void EchoLook(const FInputActionValue& Value);
	void EKeyPressed();
	void Attack();
	void Dodge();

	/*Play Montage Sections*/
	void PlayAttackMontage();

	bool CanEchoAttack();

	bool CanEchoUnEquipWeapon();

	bool CanEchoEquipWeapon();

	void PlayEquipMontage(FName SectionName);

	/*Anim Notifies*/
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void AddToSpineSocket();

	UFUNCTION(BlueprintCallable)
	void AddToHandSocket();

	UFUNCTION(BlueprintCallable)
	void EquippingEnd();
	/*Anim Notifies*/

private:
	/*States*/
	ECharacterState EchoCurrentState = ECharacterState::ECS_UnEquipped;
	EActionState EchoActionState = EActionState::EAS_Unoccupied;
	/*States*/

	/*Components*/
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* EchoSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* EchoCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	class UGroomComponent* EchoHair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* EchoEyebrows;
	/*Components*/

	/*Pointers*/
	UPROPERTY(VisibleInstanceOnly)
	class AItem* OverlappingItem;

	UPROPERTY(VisibleAnyWhere, Category = Weapon)
	class AWeapon* EquippedWeapon;
	/*Pointers*/

public: 
	/*Setters and Getters*/
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetEchoCharacterState() const { return this->EchoCurrentState; }
	/*Setters and Getters*/
};
