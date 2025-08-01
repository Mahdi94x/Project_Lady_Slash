#include "Characters/SlashCharacter.h"
#include"Components/InputComponent.h" 
#include "EnhancedInputSubsystems.h" 
#include "EnhancedInputComponent.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	EchoSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("EchoCameraBoom"));
	EchoSpringArm->SetupAttachment(GetRootComponent());
	EchoSpringArm->TargetArmLength = 300.f;
	EchoSpringArm->bUsePawnControlRotation = true;

	EchoCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("EchoCamera"));
	EchoCamera->SetupAttachment(EchoSpringArm);

	EchoHair = CreateDefaultSubobject<UGroomComponent>(TEXT("EchoHair"));
	EchoHair->SetupAttachment(GetMesh());
	EchoHair->AttachmentName = FString("head");

	EchoEyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EchoEyebrows"));
	EchoEyebrows->SetupAttachment(GetMesh());
	EchoEyebrows->AttachmentName = FString("head");

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
	const FVector2D MovementVector = Value.Get<FVector2D>(); /*2D Vector x and y*/

	const FRotator Rotation = Controller->GetControlRotation(); /*Controller Rotation*/
	const FRotator YawRotation(0.f,Rotation.Yaw, 0.f); 
	/*Rotation matrix based on the yaw of the controller rotation prependiculat to x ,y,
	ensure movement stays parallel to the ground*/

	// Find out which way is forward
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); /*forward direction in 3D cares about x*/
	AddMovementInput(ForwardDirection, MovementVector.Y); /*in 2D vector forward and backward relate to y */

	// Find out which way is right
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); /*right direction in 3D cares about y*/
	AddMovementInput(RightDirection, MovementVector.X); /*in 2D vector right and left relate to the x*/
	
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




