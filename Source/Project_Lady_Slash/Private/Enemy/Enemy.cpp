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
#include "Items/SoulsPickup/SoulsPickup.h"
#include "Items/HealthPickup/HealthPickup.h"
#include "Items/Treasure/Treasure.h"

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
	if (IsCharacterInsideAttackRadius())
	{
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsCharacterOutsideAttackRadius())
	{
		EnemyChaseTarget();
	}
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

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsEnemyDead()) { ShowHealthBar(); }
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponBoxCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (EnemySensing) { EnemySensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen); }
	InitializeEnemy();
}

void AEnemy::Die()
{
	Super::Die();

	EnemyState = EEnemyState::EES_Dead;
	ClearPatrolTimer();
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	this->SetLifeSpan(DeathLifeSpan);
	SetWeaponBoxCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyDeadDrop();
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
	Super::Attack();
	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
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

void AEnemy::InitializeEnemy()
{
	EnemyAIController = Cast<AAIController>(GetController());
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 3.f);
	HideHealthBar();
	SpawnDefaultWeapon();
	Tags.Add(FName("Enemy"));
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
	if (SeenPawn->ActorHasTag(FName("Dead"))) return;

	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		EnemyChaseTarget();
	}
}

void AEnemy::EnemyDeadDrop()
{
	UWorld* World = GetWorld();
	const FVector SpawnLocation = this->GetActorLocation() + FVector(50.f, 50.f, 75.f);

	if (World && EnemyDropItemsArray.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, 2);

		switch (Selection)
		{
			case 0: /*Souls Case*/
			{
				class ASoulsPickup* SpawnedSoul = World->SpawnActor<ASoulsPickup>(EnemyDropItemsArray[Selection], SpawnLocation, GetActorRotation());
				if (SpawnedSoul) SpawnedSoul->SetSoulValue(EnemySoulMinDrop, EnemySoulMaxDrop);
				break;
			}

			case 1: /*Health Case*/
			{
				class AHealthPickup* SpawnedHealth = World->SpawnActor<AHealthPickup>(EnemyDropItemsArray[Selection], SpawnLocation, GetActorRotation());
				if (SpawnedHealth) SpawnedHealth->SetHealthValue(EnemyHealthMinDrop, EnemyHealthMaxDrop);
				break;
			}

			case 2: /*Treasure Case*/
			{
				class ATreasure* SpawnedItem = World->SpawnActor<ATreasure>(EnemyDropItemsArray[Selection], SpawnLocation, GetActorRotation());
				break;
			}

		}
	}
}
