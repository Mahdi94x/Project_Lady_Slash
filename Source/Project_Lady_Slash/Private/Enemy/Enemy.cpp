#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Project_Lady_Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"

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
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetHealthBarPercent(1.f);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	DirectionalHitReact(ImpactPoint);

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
	return DamageAmount;
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
