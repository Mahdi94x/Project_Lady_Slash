#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Project_Lady_Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	EnemyAttributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("EnemyAttributes"));

	EnemyHealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("EnemyHealthBar"));
	EnemyHealthBar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetHealthBarPercent(0.4f);
		EnemyHealthBar->SetVisibility(false);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CombatTarget)
	{
		const double DistanceToTarget = (CombatTarget->GetActorLocation() - this->GetActorLocation()).Size();
		if (DistanceToTarget > CombatRaduis /*Lose Interest*/)
		{
			CombatTarget = nullptr;
			if (EnemyHealthBar) { EnemyHealthBar->SetVisibility(false); }
		}
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetVisibility(true);
	}

	if (EnemyAttributes && EnemyAttributes->IsCharacterAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else
	{
		Die();
	}
	
	if (HitSounds)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSounds,
			ImpactPoint
		);
	}

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

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
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

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Die()
{
	PlayDeathMontage();

	if (EnemyHealthBar) {EnemyHealthBar->SetVisibility(false);}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	this->SetLifeSpan(8.f);

}

void AEnemy::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);

		const int32 SectionSelection = FMath::RandRange(0, 5);
		FName SectionName = NAME_None;
		
		switch (SectionSelection)
		{
		case 0:
			SectionName = FName("Death0");
			EnemyDeathPose = EDeathPose::EDP_Death0;
			break;
		case 1:
			SectionName = FName("Death1");
			EnemyDeathPose = EDeathPose::EDP_Death1;
			break;
		case 2:
			SectionName = FName("Death2");
			EnemyDeathPose = EDeathPose::EDP_Death2;
			break;
		case 3:
			SectionName = FName("Death3");
			EnemyDeathPose = EDeathPose::EDP_Death3;
			break;
		case 4:
			SectionName = FName("Death4");
			EnemyDeathPose = EDeathPose::EDP_Death4;
			break;
		case 5:
			SectionName = FName("Death5");
			EnemyDeathPose = EDeathPose::EDP_Death5;
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EnemyAttributes)
	{
		EnemyAttributes->UpdateCurrentHealth(DamageAmount);

		if (EnemyHealthBar)
		{
			EnemyHealthBar->SetHealthBarPercent(EnemyAttributes->GetCurrentHealthPercent());
		}
	}

	CombatTarget = EventInstigator->GetPawn();

	return DamageAmount;
}
