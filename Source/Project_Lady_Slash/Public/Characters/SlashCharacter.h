#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
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

	UPROPERTY(EditAnywhere, Category = "Echo Input")
	class UInputMappingContext* EchoMappingContext;

	UPROPERTY(EditAnywhere, Category = "Echo Input")
	class UInputAction* EchoMovementAction;

	UPROPERTY(EditAnywhere, Category = "Echo Input")
	UInputAction* EchoLookAction;

	UPROPERTY(EditAnywhere, Category = "Echo Input")
	class UInputAction* EchoJumpAction;

	UPROPERTY(EditAnywhere, Category = "Echo Input")
	class UInputAction* EchoEKeyAction;

	UPROPERTY(EditAnywhere, Category = "Echo Input")
	class UInputAction* EchoAttackAction;

	UPROPERTY(EditAnywhere, Category = "Echo Input")
	class UInputAction* EchoDodgeAction;

	void EchoMove(const FInputActionValue& Value);
	void EchoLook(const FInputActionValue& Value);
	void EKeyPressed();
	void Attack();
	void Dodge();

private:

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* EchoSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* EchoCamera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	class UGroomComponent* EchoHair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* EchoEyebrows;


};
