#include "Enemy/Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Project_Lady_Slash/DebugMacros.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Items/Weapon/Weapon.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

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

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetHealthBarPercent(0.4f);
		EnemyHealthBar->SetVisibility(false);
	}

	EnemyAIController = Cast<AAIController>(GetController());
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, 3.f);
	
	if (EnemySensing) { EnemySensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen); }

	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->WeaponBeingEquip(this->GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;

	}
	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	if (EnemyState == EEnemyState::EES_Patrolling)
	{
		CheckPatrolTarget();
	}
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
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		// Outside combat range, lose interest back to patrolling
		CombatTarget = nullptr;
		if (EnemyHealthBar) { EnemyHealthBar->SetVisibility(false); }
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(CurrentPatrolTarget);
		UE_LOG(LogTemp, Warning, TEXT("LoseInterest, Patrol"));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		// Outside attack range , chase character if character inside combat raduis
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
		UE_LOG(LogTemp, Warning, TEXT("Chase from CheckCombatTarget()"));
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		// Inside Attack Range, Attack SlashCharacter
		EnemyState = EEnemyState::EES_Attacking;
		UE_LOG(LogTemp, Warning, TEXT("Attack"));
		/*TODO: Implement Attacking Behavior - play attack montage - cause damage to the character*/
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (EnemyHealthBar)
	{
		EnemyHealthBar->SetVisibility(true);
	}

	if (CharacterAttributes && CharacterAttributes->IsCharacterAlive())
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

void AEnemy::Die()
{
	PlayDeathMontage();

	if (EnemyHealthBar) {EnemyHealthBar->SetVisibility(false);}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	this->SetLifeSpan(5.f);

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

bool AEnemy::InTargetRange(AActor* Target, double AcceptanceRaduis)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - this->GetActorLocation()).Size();
	/*DRAW_SPHERE_SingleFrame(this->GetActorLocation());
	DRAW_SPHERE_SingleFrame(Target->GetActorLocation());*/
	return DistanceToTarget <= AcceptanceRaduis; /*return true if target is within range*/
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

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyAIController == nullptr || Target == nullptr) return;
	
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyAIController->MoveTo(MoveRequest);
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(CurrentPatrolTarget);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CharacterAttributes)
	{
		CharacterAttributes->UpdateCurrentHealth(DamageAmount);

		if (EnemyHealthBar)
		{
			EnemyHealthBar->SetHealthBarPercent(CharacterAttributes->GetCurrentHealthPercent());
		}
	}

	CombatTarget = EventInstigator->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToTarget(CombatTarget);
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

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) return;

	if (SeenPawn->ActorHasTag(FName("SlashCharacter")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		CombatTarget = SeenPawn;

		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			this->MoveToTarget(CombatTarget);
			UE_LOG(LogTemp, Warning, TEXT("Chase from PawnSeen(APawn* SeenPawn)"));
		}
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}