#include "BaseCharacter/BaseCharacter.h"
#include "Items/Weapon/Weapon.h"
#include "Components/BoxComponent.h"
#include "Components/AttributeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CharacterAttributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("CharacterAttributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponBoxCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActorsPerSwing.Empty(); /*One Hit Per Swing*/
	}
}

void ABaseCharacter::Attack()
{

}

bool ABaseCharacter::CanBaseCharacterAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{

}

bool ABaseCharacter::IsBaseCharacterAlive()
{
	return CharacterAttributes && CharacterAttributes->IsCharacterAlive();
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSounds)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSounds,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	UWorld* World = GetWorld();
	if (World && HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			World,
			HitParticle,
			ImpactPoint
		);
	}
}

void ABaseCharacter::HandleDamageBaseCharacter(float DamageAmount)
{
	if (CharacterAttributes)
	{
		CharacterAttributes->UpdateCurrentHealth(DamageAmount);
	}
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxsectionsIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxsectionsIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ABaseCharacter::Die()
{

}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	/*Calculating the angle for enemy direction hit react*/
	const FVector Forward = this->GetActorForwardVector(); // magnitude = 1, normalized
	// Lower Impact Point from BoxHit() to the enemy's actor location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal(); //magnitude = 1, normalizd

	// Forward (DOT) ToHit = |Forward||ToHit|Cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// Take the inverse cosine to get the angle
	double Theta = FMath::Acos(CosTheta);

	// convert from radians to degree
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theata should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
	{
		Theta *= -1;
	}

	FName SectionName("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		SectionName = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		SectionName = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		SectionName = FName("FromRight");
	}

	PlayHitReactMontage(SectionName);

	/*Debugging

	DRAW_SPHERE_Color(ImpactPoint, FColor::Orange);

	// Message on the viewport
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green, FString::Printf(TEXT("Theta: %f"), Theta));
	}

	// Drawing Forward Vector
	UKismetSystemLibrary::DrawDebugArrow(
		this,
		GetActorLocation(),
		GetActorLocation() + Forward * 60,
		5.f,
		FColor::Red,
		5.f);

	// Drawing ToHit Vector
	UKismetSystemLibrary::DrawDebugArrow(
		this,
		GetActorLocation(),
		GetActorLocation() + ToHit * 60,
		5.f,
		FColor::Green,
		5.f);

	UKismetSystemLibrary::DrawDebugArrow(
		this,
		GetActorLocation(),
		GetActorLocation() + CrossProduct * 100,
		5.f,
		FColor::Blue,
		5.f);
	Debugging*/
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}