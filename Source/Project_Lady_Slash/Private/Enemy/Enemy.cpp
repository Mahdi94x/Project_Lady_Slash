#include "Enemy/Enemy.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapon/Weapon.h"
#include "Project_Lady_Slash/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	EnemyHealthBar = CreateDefaultSubobject<UHealthBarComponent>(TEXT("EnemyHealthBar"));
	EnemyHealthBar->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	EnemySensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("EnemyPawnSensingComponent"));
	EnemySensing->SetPeripheralVisionAngle(45.f);
	EnemySensing->SightRadius = 4000.f;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsEnemyDead()) return;

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamageBaseCharacter(DamageAmount);
	CombatTarget = EventInstigator->GetPawn();
	EnemyChaseTarget();
	UE_LOG(LogTemp, Warning, TEXT("Chase From TakeDamage()"));

	return DamageAmount;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	ShowHealthBar();

	if (IsBaseCharacterAlive()) { DirectionalHitReact(ImpactPoint); }
	else { Die(); }

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (EnemySensing) { EnemySensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen); }
	InitializeEnemy();
}

void AEnemy::Die()
{
	EnemyState = EEnemyState::EES_Dead;
	ClearPatrolTimer();
	ClearAttackTimer();
	PlayDeathMontage();
	HideHealthBar();
	DisableCapsule();
	this->SetLifeSpan(DeathLifeSpan);
	//GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AEnemy::HandleDamageBaseCharacter(float DamageAmount)
{
	Super::HandleDamageBaseCharacter(DamageAmount);

	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetHealthBarPercent(CharacterAttributes->GetCurrentHealthPercent());
	}
}

void AEnemy::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;
	Super::Attack();
	this->PlayAttackMontage();
}

bool AEnemy::CanBaseCharacterAttack()
{
	bool bCanEnemyAttack = IsCharacterInsideAttackRadius() &&
		!IsEnemyAttacking() &&
		!IsEnemyDead() &&
		!IsEnemyEngaged();
	return bCanEnemyAttack;
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

int32 AEnemy::PlayDeathMontage()
{
	const int32 Selection = Super::PlayDeathMontage();
	TEnumAsByte<EDeathPose> DeathPose(Selection);
	if (DeathPose < EDeathPose::EDP_MAX)
	{
		EnemyDeathPose = DeathPose;
	}
	return Selection;
}

void AEnemy::InitializeEnemy()
{
	EnemyAIController = Cast<AAIController>(GetController());
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 3.f);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(CurrentPatrolTarget, PatrolRaduis))
	{
		CurrentPatrolTarget = SelectPatrolTarget();
		const float RandomWaitTime = FMath::RandRange(WaitTimeMin, WaitTimeMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, RandomWaitTime);
	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsCharacterOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEnemyEngaged())
		{
			EnemyStartPatrolling();
		}
		UE_LOG(LogTemp, Warning, TEXT("LoseInterest, Patrol"));
	}
	else if (IsCharacterOutsideAttackRadius() && !IsEnemyChasing())
	{
		ClearAttackTimer();
		if (!IsEnemyEngaged())
		{
			EnemyChaseTarget();
		}
		UE_LOG(LogTemp, Warning, TEXT("Chase from CheckCombatTarget()"));
	}
	else if (CanBaseCharacterAttack())
	{
		StartEnemyAttackTimer();
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

bool AEnemy::IsEnemyDead()
{
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEnemyEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

bool AEnemy::IsEnemyAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsCharacterInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsEnemyChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsCharacterOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

void AEnemy::EnemyChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsCharacterOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);
}

void AEnemy::EnemyStartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(CurrentPatrolTarget);
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::HideHealthBar()
{
	if (EnemyHealthBar) { EnemyHealthBar->SetVisibility(false); }
}

void AEnemy::ShowHealthBar()
{
	if (EnemyHealthBar) { EnemyHealthBar->SetVisibility(true); }
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(EnemyAttackTimer);
}

void AEnemy::StartEnemyAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float EnemyAttackTime = FMath::RandRange(EnemyAttackMin, EnemyAttackMax);
	GetWorldTimerManager().SetTimer(EnemyAttackTimer, this, &AEnemy::Attack, EnemyAttackTime);
}

bool AEnemy::InTargetRange(AActor* Target, double AcceptanceRaduis)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - this->GetActorLocation()).Size();
	/*DRAW_SPHERE_SingleFrame(this->GetActorLocation());
	DRAW_SPHERE_SingleFrame(Target->GetActorLocation());*/
	return DistanceToTarget <= AcceptanceRaduis; /*return true if target is within range*/
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyAIController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyAIController->MoveTo(MoveRequest);
}

AActor* AEnemy::SelectPatrolTarget()
{
	TArray<AActor*> ValidPatrolTargets;
	for (auto Target : PatrolTargets)
	{
		if (Target != CurrentPatrolTarget)
		{
			ValidPatrolTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidPatrolTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, ValidPatrolTargets.Num() - 1);
		return ValidPatrolTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->WeaponBeingEquip(this->GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("SlashCharacter"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		EnemyChaseTarget();
	}
}
