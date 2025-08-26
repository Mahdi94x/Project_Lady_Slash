#include "Characters/SlashCharacter.h"
#include "Components/InputComponent.h" 
#include "EnhancedInputSubsystems.h" 
#include "EnhancedInputComponent.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Weapon/Weapon.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

/*Constructor*/
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

/*BeginPlay*/
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
	Tags.Add(FName("SlashCharacter"));
}

/*PlayerInput*/
void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(EchoMovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EchoMove);
		EnhancedInputComponent->BindAction(EchoLookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EchoLook);
		EnhancedInputComponent->BindAction(EchoJumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(EchoEKeyAction, ETriggerEvent::Triggered, this, &ASlashCharacter::EKeyPressed);
		EnhancedInputComponent->BindAction(EchoAttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(EchoDodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);
	}
}

/*Tick Function*/
void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*Movement and Looking*/
void ASlashCharacter::EchoMove(const FInputActionValue& Value)
{
	if (EchoActionState != EActionState::EAS_Unoccupied) {return;} 
	const FVector2D MovementVector = Value.Get<FVector2D>(); /*2D Vector x and y*/

	const FRotator Rotation = Controller->GetControlRotation(); /*Controller Rotation*/
	const FRotator YawRotation(0.f,Rotation.Yaw, 0.f); 
	/*Rotation matrix based on the yaw of the controller rotation perpendicular to x ,y,
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

/*Overlapping - Equipping - Disarming*/
void ASlashCharacter::EKeyPressed()
{
	/*Overlapping with a weapon*/
	if (OverlappingItem)
	{
		AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
		if (OverlappingWeapon)
		{
			OverlappingWeapon->WeaponBeingEquip(this->GetMesh(), FName("RightHandSocket"), this, this);
			EchoCurrentState = ECharacterState::ECS_EquippedOneHanded;
			OverlappingItem = nullptr; /*setting the overlapped item to nullptr to avoid reoverlapping with the same weapon equipped*/
			EquippedWeapon = OverlappingWeapon;
			if (EquipSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					this,
					EquipSound,
					this->GetActorLocation()
				);
			}
		}
	}
	/*After Picking Up The Weapon*/
	else
	{
		if (CanEchoUnEquipWeapon())
		{
			FName SectionName = FName("EquipToBack");
			PlayEquipMontage(SectionName);
			EchoCurrentState = ECharacterState::ECS_UnEquipped;
			EchoActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanEchoEquipWeapon())
		{
			FName SectionName = FName("EquipToHand");
			PlayEquipMontage(SectionName);
			EchoCurrentState = ECharacterState::ECS_EquippedOneHanded;
			EchoActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

bool ASlashCharacter::CanEchoUnEquipWeapon()
{
	return EchoActionState == EActionState::EAS_Unoccupied &&
		EchoCurrentState != ECharacterState::ECS_UnEquipped;
}

bool ASlashCharacter::CanEchoEquipWeapon()
{
	return EchoActionState == EActionState::EAS_Unoccupied &&
		EchoCurrentState == ECharacterState::ECS_UnEquipped &&
		EquippedWeapon;
}

void ASlashCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void ASlashCharacter::AddToSpineSocket()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachWeaponMeshToSocket(this->GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::AddToHandSocket()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachWeaponMeshToSocket(this->GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::EquippingEnd()
{
	EchoActionState = EActionState::EAS_Unoccupied;
}

/*Attacking*/
void ASlashCharacter::PlayAttackMontage()
{
	Super::PlayAttackMontage();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 SectionSelection = FMath::RandRange(0, 1);
		FName SectionName = NAME_None;

		switch (SectionSelection)
		{
		case 0:
			SectionName = FName("Attack0");
			break;
		case 1:
			SectionName = FName("Attack1");
			break;

		default:
			break;
		}

		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

bool ASlashCharacter::CanCharacterAttack()
{
	return EchoActionState == EActionState::EAS_Unoccupied && 
		EchoCurrentState != ECharacterState::ECS_UnEquipped;
}

void ASlashCharacter::Attack()
{
	Super::Attack();
	if (CanCharacterAttack())
	{
		PlayAttackMontage();
		EchoActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::AttackEnd()
{
	EchoActionState = EActionState::EAS_Unoccupied;
}

/*Dodge*/
void ASlashCharacter::Dodge()
{
	UE_LOG(LogTemp, Warning, TEXT("Echo::Dodge"));
}

/*Jumping*/
void ASlashCharacter::Jump()
{
	Super::Jump();
}
