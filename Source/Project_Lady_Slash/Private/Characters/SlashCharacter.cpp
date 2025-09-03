#include "Characters/SlashCharacter.h"
#include "Components/InputComponent.h" 
#include "EnhancedInputSubsystems.h" 
#include "EnhancedInputComponent.h" 
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "Components/AttributeComponent.h"

/*Constructor*/
ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

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

	InitializeEnhancedInput(PlayerController);
	InitializeSlashOverlay(PlayerController);
	Tags.Add(FName("EngageableTarget"));
}

/*PlayerEnhancedInput*/
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

void ASlashCharacter::InitializeEnhancedInput(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		/* Enhanced Input*/
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(EchoMappingContext, 0);
		}
	}
}

/*Movement and Looking*/
void ASlashCharacter::EchoMove(const FInputActionValue& Value)
{
	if (!IsEchoUnoccupied()) {return;} 
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
			EchoEquippingTheWeapon(OverlappingWeapon);
			EchoPlayEquipSound();
		}
	}
	/*After Picking Up The Weapon*/
	else
	{
		if (CanEchoUnEquipWeapon())
		{
			EchoDisarmTheWepaon();
		}
		else if (CanEchoEquipWeapon())
		{
			EchoArmTheWeapon();
		}
	}
}

void ASlashCharacter::EchoArmTheWeapon()
{
	FName SectionName = FName("EquipToHand");
	PlayEquipMontage(SectionName);
	EchoCurrentState = ECharacterState::ECS_EquippedOneHanded;
	EchoActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::EchoDisarmTheWepaon()
{
	FName SectionName = FName("EquipToBack");
	PlayEquipMontage(SectionName);
	EchoCurrentState = ECharacterState::ECS_UnEquipped;
	EchoActionState = EActionState::EAS_EquippingWeapon;
}

void ASlashCharacter::EchoPlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			this->GetActorLocation()
		);
	}
}

void ASlashCharacter::EchoEquippingTheWeapon(AWeapon* Weapon)
{
	Weapon->WeaponBeingEquip(this->GetMesh(), FName("RightHandSocket"), this, this);
	EchoCurrentState = ECharacterState::ECS_EquippedOneHanded;
	OverlappingItem = nullptr; /*setting the overlapped item to nullptr to avoid reoverlapping with the same weapon equipped*/
	EquippedWeapon = Weapon;
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
/*Overlapping - Equipping - Disarming*/

/*Attacking*/
bool ASlashCharacter::CanBaseCharacterAttack()
{
	return EchoActionState == EActionState::EAS_Unoccupied && 
		EchoCurrentState != ECharacterState::ECS_UnEquipped;
}

void ASlashCharacter::Attack()
{
	Super::Attack();
	if (CanBaseCharacterAttack())
	{
		PlayAttackMontage();
		EchoActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::AttackEnd()
{
	EchoActionState = EActionState::EAS_Unoccupied;
}
/*Attacking*/

/*Dodge*/
void ASlashCharacter::Dodge()
{
	UE_LOG(LogTemp, Warning, TEXT("Echo::Dodge"));
}

/*Jumping*/
void ASlashCharacter::Jump()
{
	if (IsEchoUnoccupied())
	{
		Super::Jump();
	}
}

/*GitHit Implementation - TakeDamage - HitReacting*/
float ASlashCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamageBaseCharacter(DamageAmount);
	SetEchoHUDHealth();
	return DamageAmount;
}

void ASlashCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	SetWeaponBoxCollisionEnabled(ECollisionEnabled::NoCollision); /*in case AttackMontage got interupted before DisablingBoxCollision*/

	if (IsBaseCharacterAlive())
	{
		EchoActionState = EActionState::EAS_HitReaction;
	}
}

void ASlashCharacter::HitReactingEnd()
{
	EchoActionState = EActionState::EAS_Unoccupied;
}

/*Die Function*/
void ASlashCharacter::Die()
{
	Super::Die();
	EchoActionState = EActionState::EAS_Dead;
	DisableMeshCollision();
}

/*Slash HUD & Overlay*/
void ASlashCharacter::InitializeSlashOverlay(APlayerController* PlayerController)
{
	if (PlayerController)
	{
		/*HUD*/
		ASlashHUD* SlashHUD = Cast<ASlashHUD>(PlayerController->GetHUD());
		if (SlashHUD)
		{
			SlashOverlay = SlashHUD->GetSlashOverlay();
			SetEchoHUDHealth();
			if (SlashOverlay && CharacterAttributes)
			{
				SlashOverlay->SetEchoStaminaProgressBarPercent(1.f);
				SlashOverlay->SetSoulsCountText(0);
				SlashOverlay->SetCoinsCountText(0);
			}
		}
	}
}

void ASlashCharacter::SetEchoHUDHealth()
{
	if (SlashOverlay && CharacterAttributes)
	{
		SlashOverlay->SetEchoHealthProgressBarPercent(CharacterAttributes->GetCurrentHealthPercent());
	}
}

/* Helper Functions*/
bool ASlashCharacter::IsEchoUnoccupied()
{
	return EchoActionState == EActionState::EAS_Unoccupied;
}

