#include "Characters/SlashAnimInstance.h"
#include "Characters/SlashCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	EchoCharacter = Cast<ASlashCharacter>(TryGetPawnOwner());
	if (EchoCharacter)
	{
		EchoMovementComponent = EchoCharacter->GetCharacterMovement();
	}
}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (EchoMovementComponent)
	{
		EchoGroundSpeed = UKismetMathLibrary::VSizeXY(EchoMovementComponent->Velocity);
		IsEchoFalling = EchoMovementComponent->IsFalling();
		EchoCurrentStateAnim = EchoCharacter->GetEchoCharacterState();
	}
}
