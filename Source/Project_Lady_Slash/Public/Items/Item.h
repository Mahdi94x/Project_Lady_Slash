#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

/*Raw C++ Enum*/
enum class EItemState : uint8
{
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped")
};

UCLASS()
class PROJECT_LADY_SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
	

protected:
	virtual void BeginPlay() override;
	void SpawnPickupEffect();
	void PlayPickupSound();
	UFUNCTION(BlueprintPure)
	float TransformedSine();
	UFUNCTION(BlueprintPure)
	float TransformedCosine();

	EItemState ItemCurrentState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) // Details and Component Panels
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* OverlapSphere;

	UPROPERTY(EditAnywhere, Category = "Item Parameters", BlueprintReadWrite)
	float Amplitude = 2.5f;
	
	UPROPERTY(EditAnywhere, Category = "Item Parameters", BlueprintReadWrite)
	float TimeConstant = 10.f;

	UPROPERTY(EditAnywhere, Category = "Item Parameters", BlueprintReadWrite)
	float RotationRate = 100.f;

	UPROPERTY(EditAnywhere, Category = "Item Parameters")
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere, Category = "Item Parameters")
	USoundBase* PickupSound;

	UFUNCTION()
	virtual void OnSphereBeginOverlap (UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ItemEffect;


private:

	void ItemDriftToGround();

	UPROPERTY(VisibleInstanceOnly, Category = "Item Parameters", BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	double RunningTime = 0;

	UPROPERTY(VisibleAnywhere, Category = "Item Parameters")
	double DesiredZ;

	UPROPERTY(EditAnywhere, Category = "Item Parameters")
	double DriftRate = -20.f;

	UPROPERTY(EditAnywhere, Category = "Item Parameters")
	double AboveTheGroundValue = 125.f;

};