#include "Characters/SlashCharacter.h"
#include"Components/InputComponent.h" 
#include "EnhancedInputSubsystems.h" 
#include "EnhancedInputComponent.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	EchoSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("EchoCameraBoom"));
	EchoSpringArm->SetupAttachment(GetRootComponent());
	EchoSpringArm->TargetArmLength = 300.f;
	EchoSpringArm->bUsePawnControlRotation = true;


	EchoCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("EchoCamera"));
	EchoCamera->SetupAttachment(EchoSpringArm);

}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(EchoMappingContext , 0);
		}
	}
	
}

void ASlashCharacter::EchoMove(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
	
}

void ASlashCharacter::EchoLook(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ASlashCharacter::EKeyPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("Echo::EKeyPressed"));
}

void ASlashCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Echo::Attack"));
}

void ASlashCharacter::Dodge()
{
	UE_LOG(LogTemp, Warning, TEXT("Echo::Dodge"));
}

void ASlashCharacter::Jump()
{
	Super::Jump();
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(EchoMovementAction,ETriggerEvent::Triggered, this, &ASlashCharacter::EchoMove);
		EnhancedInputComponent->BindAction(EchoLookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EchoLook);
		EnhancedInputComponent->BindAction(EchoJumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EchoEKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(EchoAttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(EchoDodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
	}
}




