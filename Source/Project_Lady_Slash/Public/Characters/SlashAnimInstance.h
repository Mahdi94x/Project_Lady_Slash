#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "SlashAnimInstance.generated.h"

UCLASS()
class PROJECT_LADY_SLASH_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Animation Variables")
	class ASlashCharacter* EchoCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Animation Variables")
	class UCharacterMovementComponent* EchoMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Animation Variables")
	float EchoGroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation Variables")
	bool IsEchoFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Animation Variables")
	ECharacterState EchoCurrentStateAnim;
	
};
