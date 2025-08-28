#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"


UCLASS()
class PROJECT_LADY_SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()

public:

	AWeapon();
	virtual void Tick(float DeltaTime) override;
	void WeaponBeingEquip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void DeactivateEmbers();
	void DisableSphereCollision();
	void AttachWeaponMeshToSocket(USceneComponent* InParent, const FName& InSocketName);
	TArray<AActor*> IgnoreActorsPerSwing; /*One Hit Per Swing*/

protected:

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnWeaponBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool ActorIsSameType(AActor* OtherActor);
	void ExecuteGetHit(FHitResult& BoxHit);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateField(const FVector& FieldLocation);

private:
	void BoxTrace(FHitResult& BoxHit);
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	class UBoxComponent* WeaponBox;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* BoxTraceEnd;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float WeaponDamage = 20.f;
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;


public:
	/*Setters and Getters*/
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return this->WeaponBox; }
};
